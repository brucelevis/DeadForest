#pragma once
#include "ThreadSafe_List.h"

namespace realtrick
{

	template<typename Key, typename Value, typename Hash = std::hash<Key> >
	class ThreadSafe_Map
	{
	private:
		class bucket_type
		{
			friend class ThreadSafe_Map;

			using bucket_value = std::pair<Key, Value>;
			using bucket_data = ThreadSafe_List<bucket_value>;
			bucket_data _data;
			mutable boost::shared_mutex _mutex;

			std::shared_ptr<bucket_value> find_entry_for(Key const& key)
			{
				return _data.find_first_if(
					[&](bucket_value const& item)
				{return item.first == key; });
			}

		public:

			template<typename Function>
			void for_each(Function f)
			{
				return _data.for_each(f);
			}

			Value value_for(Key const& key, Value const& default_value)
			{
				boost::shared_lock<boost::shared_mutex> lock(_mutex);

				std::shared_ptr<bucket_value> const foundEntry = find_entry_for(key);
				return (foundEntry == nullptr) ?
					default_value : foundEntry->second;
			}

			void add_or_update_mapping(Key const& key, Value const& value)
			{
				std::unique_lock<boost::shared_mutex> lock(_mutex);
				std::shared_ptr<bucket_value> const foundEntry = find_entry_for(key);
				if (foundEntry == nullptr)
				{
					_data.push_front(bucket_value(key, value));
				}
				else
				{
					foundEntry->second = value;
				}
			}

			void remove_mapping(Key const& key)
			{
				std::unique_lock<boost::shared_mutex> lock(_mutex);

				_data.remove_if([&](bucket_value const& item) {
					return item.first == key;
				});

			}
		};



		bucket_type& get_bucket(Key const& key) const
		{
			std::size_t const bucket_index = _hasher(key) % _buckets.size();
			return *_buckets[bucket_index];
		}

	public:
		ThreadSafe_Map(
			unsigned num_buckets = 19, Hash const& hasher = Hash()) :
			_buckets(num_buckets), _hasher(hasher)
		{
			for (unsigned i = 0; i < num_buckets; ++i)
			{
				_buckets[i].reset(new bucket_type);
			}
		}

		ThreadSafe_Map(ThreadSafe_Map const& other) = delete;
		ThreadSafe_Map& operator=(ThreadSafe_Map const& other) = delete;

		Value value_for(Key const& key,
			Value const& default_value = Value()) const
		{
			return get_bucket(key).value_for(key, default_value);
		}

		void add_or_update_mapping(Key const& key, Value const& value)
		{
			get_bucket(key).add_or_update_mapping(key, value);
		}

		void remove_mapping(Key const& key)
		{
			get_bucket(key).remove_mapping(key);
		}


		std::map<Key, Value> get_map() const
		{
			std::vector<boost::shared_lock<boost::shared_mutex> > locks;
			for (unsigned i = 0; i < _buckets.size(); ++i)
			{
				locks.push_back(
					boost::shared_lock<boost::shared_mutex>(_buckets[i]->_mutex));
			}
			std::map<Key, Value> res;
			for (int i = 0; i < _buckets.size(); ++i)
			{
				_buckets[i]->for_each([&](typename bucket_type::bucket_value& item) {
					res.insert(item);
				});
			}
			return res;
		}
	private:
		std::vector<std::unique_ptr<bucket_type> > _buckets;
		Hash _hasher;
	};

}