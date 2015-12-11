#include "PlanetGroundComponent.h"
#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "CircleColliderComponent.h"
#include "PlanetComponent.h"
#include "SystemLog.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {

		const Vector2D<double> PlanetGroundComponent::GetDownwardDirectionVector(const Vector2D<double>& pos)const
		{
			return Vector2D<double>::GetParallelUnitVector(game_object().transform().global_position() - pos);
		}

		const double PlanetGroundComponent::GetAltitude(const Vector2D<double>& position)const
		{
			auto center_to_pos_vec = position - game_object().transform().global_position();
			double distance = center_to_pos_vec.Length();
			double height = planet_component_->GetHeightByRad(std::atan2(center_to_pos_vec.y, center_to_pos_vec.x));
			return distance - height;
		}

		bool PlanetGroundComponent::OnInitialized()
		{
			if (GroundComponent::OnInitialized() == false) {
				debug::SystemLog::instance().LogError("GroundComponent�̏������Ɏ��s���܂����B", "PlanetGroundComponent::Initialize_");
				return false;
			}
			else {
				planet_component_ = game_object().GetComponent<PlanetComponent>();
				if (!planet_component_) {
					debug::SystemLog::instance().LogError("PlanetComponent���擾�ł��܂���ł����B", "PlanetGroundComponent::Initialize_");
					return false;
				}
				else {
					return true;
				}
			}
		}

		bool PlanetGroundComponent::CollideWith(components::CircleColliderComponent& collider) {
			TransformComponent& transform = game_object().transform();
			auto collider_pos = collider.GetCollisionCenterPosition();
			//�f�����S����̃R���C�_�[�̕��������߂�
			auto collider_relative_vec = collider_pos - transform.global_position();
			double collider_direction = std::atan2(collider_relative_vec.y, collider_relative_vec.x);
			//�f�����S����R���C�_�[�܂ł̋�����2��
			double collider_distance_s = Vector2D<double>::Length_Square(collider_relative_vec);
			//�Փ˔���
			if (std::pow(planet_component_->GetHeightByRad(collider_direction) + collider.radius()*collider.GetCollisionScale(), 2) > collider_distance_s) {
				auto& collider_transform = collider.game_object().transform();
				//���x�̏C��
				Vector2D<double> parallel_unit_vec(std::cos(collider_direction + math::constant::PI / 2.0), std::sin(collider_direction + math::constant::PI / 2.0));
				collider_transform.velocity(parallel_unit_vec*Vector2D<double>::Dot(collider_transform.velocity(), parallel_unit_vec));
				//�����o��
				collider_transform.global_position(transform.global_position() + Vector2D<double>::GetParallelUnitVector(collider_relative_vec)*(planet_component_->GetHeightByRad(collider_direction) + collider.radius()*collider.GetCollisionScale()));
				return true;
			} else {
				return false;
			}
		}
	}
}