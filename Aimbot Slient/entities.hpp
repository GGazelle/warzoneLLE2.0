#pragma once

#include "sdk.hpp"
#include "items.hpp"
#include "solver.hpp"

#include <string>
#include <memory>

class GameProcess;

class BaseEntity {
public:
	explicit BaseEntity(uint64_t address);
	virtual ~BaseEntity() = default;
	virtual void update(const GameProcess& process) = 0;
public:
	uint64_t address;
	EHandle handle;
};

class PlayerEntity : public BaseEntity {
public:
	explicit PlayerEntity(uint64_t entity_ptr);

	void update(const GameProcess& process) override;

	inline bool is_onground() const {
		return (flags & 0x1) != 0;
	}
	inline bool is_ducking() const {
		return (flags & 0x2) != 0;
	}
	inline bool is_alive() const {
		return life_state == 0;
	}
	inline bool is_downed() const {
		return bleedout_state != 0;
	}
	inline EHandle active_weapon() const {
		return latest_primary_weapons[0];
	}
	Vec3 get_bone_pos(size_t bone) const;
	bool is_visible(float curtime) const;

public:
	Vec3 origin;
	Vec3 angles;
	Vec3 velocity;

	EHandle ground_entity;

	int32_t health, max_health;
	int32_t shields, max_shields;
	int32_t helmet_type, armor_type;

	int32_t team_num;
	uint32_t flags;
	uint8_t life_state;
	int32_t bleedout_state;

	EHandle latest_primary_weapons[2];

	std::string model_name;
	std::unique_ptr<Mat3x4[]> bones;

	int32_t observer_mode;
	EHandle observer_target;

	float last_visible_time;

	bool zooming;
	float zoom_base_frac;
	float zoom_base_time;
	float zoom_full_start_time;

	// Local player only
	Vec3 camera_origin;
	Vec3 camera_angles;
};

class BaseNPCEntity : public BaseEntity {
public:
	explicit BaseNPCEntity(uint64_t entity_ptr);
	void update(const GameProcess& process) override;

	Vec3 get_bone_pos(size_t bone) const;
	bool is_visible(float curtime) const;

public:
	Vec3 origin;
	Vec3 angles;
	std::string model_name;
	std::unique_ptr<Mat3x4[]> bones;
	float last_visible_time;
};

class WeaponXEntity : public BaseEntity, public ProjectileWeapon {
public:
	explicit WeaponXEntity(uint64_t entity_ptr);
	void update(const GameProcess& process) override;

	float get_projectile_speed() const;
	float get_projectile_gravity() const;

public:
	EHandle weapon_owner;
	WeaponIndex weapon_index;
	int32_t ammo_in_clip;
	int32_t ammo_in_stockpile;
	int32_t lifetime_shots;
	float time_weapon_idle;
	int32_t weap_state;
	bool discarded;
	bool in_reload;
	float cur_zoom_fov;
	float target_zoom_fov;
	float projectile_scale;
	float projectile_speed;
};

class PropSurvivalEntity : public BaseEntity {
public:
	explicit PropSurvivalEntity(uint64_t entity_ptr);
	void update(const GameProcess& process) override;

public:
	Vec3 origin;
	int32_t ammo_in_clip;
	ItemID custom_script_int;
	uint32_t survival_property;
	WeaponIndex weapon_index;
	uint32_t mod_bit_field;
};

class WorldEntity : public BaseEntity {
public:
	explicit WorldEntity(uint64_t entity_ptr);
	void update(const GameProcess& process) override;

	// Returns the radius of the death field.
	float death_field_radius(float curtime) const;
public:
	bool death_field_is_active;
	Vec3 death_field_origin;
	float death_field_radius_start;
	float death_field_radius_end;
	float death_field_time_start;
	float death_field_time_end;
};
