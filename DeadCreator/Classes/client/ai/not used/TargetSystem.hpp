#pragma once

#include "EntityBase.hpp"

namespace realtrick
{
	namespace client
	{

		class TargetSystem
		{
			int getTargetId() const
			{
				return _targetId;
			}


			void setTargetId(int targetId)
			{
				_targetId = targetId;
			}


			bool getAttackable() const
			{
				return _attackAble;
			}

			void setAttackable(bool attackable)
			{
				_attackAble = attackable;
			}


			bool getViewable() const
			{
				return _viewAble;
			}

			void setViewable(bool viewAble)
			{
				_viewAble = viewAble;
			}



			int getViewRange() const
			{
				return _viewRange;
			}

			void setViewRange(int viewRange)
			{
				_viewRange = viewRange;
			}


			int getAttackRange() const
			{
				return _attackRange;
			}

			void setAttackRange(int attackRange)
			{
				_attackRange = attackRange;
			}



			TargetSystem(
				EntityBase* entity,
				int attackRange,
				int viewRange)
				:
				_entity(entity),
				_targetId(EntityManager.InvalidateId),
				_attackAble(false),
				_viewAble(false),
				_attackRange(attackRange),
				_viewRange(viewRange)
			{
				//Debug.Assert(entity is ITargetable, "entity is not ITargetable!");
			}

			virtual EntityBase* UpdateTarget()
			{
				EntityBase* targetEnt = _entity.Engine.EntityMgr.GetEntity(TargetId);

				// If target presents..
				if (targetEnt != null && targetEnt.IsAlive())
				{
					Attackable = IMath.InRange(_entity.Pos, targetEnt.Pos, AttackRange);
					Viewable = IMath.InRange(_entity.Pos, targetEnt.Pos, ViewRange);
					return targetEnt;
				}
				// Otherwise, we have to set a new target.
				else
				{
					int distanceSq = -1;
					targetEnt = _entity.Engine.World.ClosestEntityFromPos(_entity.Pos, out distanceSq, _entity.Team);

					if (targetEnt == null)
					{
						Attackable = false;
						Viewable = false;
						TargetId = EntityManager.InvalidateId;
						return null;
					}

					if (distanceSq < IMath.Square(ViewRange))
					{
						TargetId = targetEnt.Id;
						Attackable = IMath.InRange(_entity.Pos, targetEnt.Pos, AttackRange);
						Viewable = IMath.InRange(_entity.Pos, targetEnt.Pos, ViewRange);
						return targetEnt;
					}
					else
					{
						Attackable = false;
						Viewable = false;
						TargetId = EntityManager.InvalidateId;
						return null;
					}
				}
			}

			bool IsTargetPresent()
			{
				return _entity.Engine.EntityMgr.Exists(_targetId);
			}

		private:
			EntityBase* _entity;
			int _targetId;
			bool _attackAble;
			bool _viewAble;
			int _attackRange;
			int _viewRange;
		}
	}
}