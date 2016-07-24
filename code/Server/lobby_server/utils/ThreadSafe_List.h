#pragma once

#include <mutex>

namespace realtrick
{
	template<typename T>
	class ThreadSafe_List
	{
	public:
		struct node
		{
			std::mutex m;
			std::shared_ptr<T> data;
			std::unique_ptr<node> next;

			node()
				: next()
			{
			}

			node(T const& value)
				: data(std::make_shared<T>(value))
			{

			}

		};

	public:
		ThreadSafe_List() {}
		~ThreadSafe_List()
		{
			remove_if([](node const&) {return true; });
		}
		ThreadSafe_List(ThreadSafe_List const& other) = delete;
		ThreadSafe_List& operator=(ThreadSafe_List const& other) = delete;

		void push_front(T const& value)
		{
			std::unique_ptr<node> newNode(new node(value));
			std::lock_guard<std::mutex> lock(_head.m);
			
			newNode->next = std::move(_head.next);
			_head.next = std::move(newNode);
		}

		template<typename Function>
		void for_each(Function f)
		{
			node* current = &_head;
			std::unique_lock<std::mutex> lock(_head.m);

			while (node* const next = current->next.get())
			{
				std::unique_lock<std::mutex> nextLock(next->m);
				lock.unlock();
				f(*next->data);
				current = next;
				lock = std::move(nextLock);
			}

		}

		template<typename Predicate>
		std::shared_ptr<T> find_first_if(Predicate p)
		{
			node* current = &_head;
			std::unique_lock<std::mutex> lock(_head.m);

			while (node* const next = current->next.get())
			{
				std::unique_lock<std::mutex> nextLock(next->m);
				lock.unlock();

				if (p(*next->data))
				{
					return next->data;
				}
				current = next;
				lock = std::move(nextLock);
			}
			return std::shared_ptr<T>();
		}
		template<typename Predicate>
		void remove_if(Predicate p)
		{
			node* current = &_head;
			std::unique_lock<std::mutex> lock(_head.m);

			while (node* const next = current->next.get())
			{
				std::unique_lock<std::mutex> nextLock(next->m);
				if (p(*next->data))
				{
					std::unique_ptr<node> oldNext = std::move(current->next);
					current->next = std::move(next->next);
					nextLock.unlock();
				}
				else
				{
					lock.unlock();
					current = next;
					lock = std::move(nextLock);
				}
			}
		}
	private:
		node _head;
	};
}