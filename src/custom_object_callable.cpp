/*
	Copyright (C) 2003-2013 by David White <davewx7@gmail.com>
	
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "asserts.hpp"
#include "custom_object_callable.hpp"
#include "formula_object.hpp"
#include "level.hpp"

namespace {
std::vector<custom_object_callable::entry>& global_entries() {
	static std::vector<custom_object_callable::entry> instance;
	return instance;
}

std::map<std::string, int>& keys_to_slots() {
	static std::map<std::string, int> instance;
	return instance;
}

const custom_object_callable* instance_ptr = NULL;

}

const custom_object_callable& custom_object_callable::instance()
{
	if(instance_ptr) {
		return *instance_ptr;
	}

	static const boost::intrusive_ptr<const custom_object_callable> obj(new custom_object_callable(true));
	return *obj;
}

namespace {
struct Property {
	std::string id, type;
};
}

custom_object_callable::custom_object_callable(bool is_singleton)
{
	if(is_singleton) {
		instance_ptr = this;
		set_type_name("custom_obj");
	}

	static const Property CustomObjectProperties[] = {
	{ "value", "any" },
	{ "_data", "any" },
	{ "consts", "any" },
	{ "type", "any" },
	{ "active", "any" },
	{ "lib", "any" },

	{ "time_in_animation", "int" },
	{ "time_in_animation_delta", "int" },
	{ "frame_in_animation", "int" },
	{ "level", "any" },

	{ "animation", "string" },
	{ "available_animations", "[string]" },

	{ "hitpoints", "int" },
	{ "max_hitpoints", "int" },
	{ "mass", "int" },
	{ "label", "string" },
	{ "x", "int" },
	{ "y", "int" },
	{ "xy", "[int]" },
	{ "z", "int" },

	{ "relative_x", "int" },
	{ "relative_y", "int" },
	{ "spawned_by", "null|custom_obj" },
	{ "spawned_children", "[custom_obj]" },

	{ "parent", "null|custom_obj" },
	{ "pivot", "string" },
	{ "zorder", "int" },
	{ "zsub_order", "int" },

	{ "previous_y", "int" },
	{ "x1", "int" },
	{ "x2", "int" },
	{ "y1", "int" },
	{ "y2", "int" },
	{ "w", "int" },
	{ "h", "int" },
	{ "mid_x", "int" },
	{ "mid_y", "int" },
	{ "mid_xy", "int" },
	{ "midpoint_x", "int" },
	{ "midpoint_y", "int" },
	{ "midpoint_xy", "int" }, 

	{ "solid_rect", "object" },
	{ "solid_mid_x", "int" },
	{ "solid_mid_y", "int" },
	{ "solid_mid_xy", "int" }, 

	{ "img_mid_x", "int" },
	{ "img_mid_y", "int" },
	{ "img_mid_xy", "int" },
	{ "img_w", "int" },
	{ "img_h", "int" },
	{ "img_wh", "int" },
	{ "front", "int" },
	{ "back", "int" },
	{ "cycle", "int" },
	{ "facing", "int" },
	
	{ "upside_down", "int" },
	{ "up", "int" },
	{ "down", "int" },
	{ "velocity_x", "int" },
	{ "velocity_y", "int" },
	{ "velocity_xy", "int" }, 

	{ "velocity_magnitude", "decimal" },
	{ "velocity_angle", "decimal" },

	{ "accel_x", "int" },
	{ "accel_y", "int" },
	{ "accel_xy", "int" },
	{ "gravity_shift", "int" },
	{ "platform_motion_x", "int" },

	{ "registry", "object" },
	{ "globals", "object" },
	{ "vars", "object" },
	{ "tmp", "object" },
	{ "group", "int" },
	{ "rotate", "decimal" },

	{ "me", "any" },
	{ "self", "any" },

	{ "red", "int" },
	{ "green", "int" },
	{ "blue", "int" },
	{ "alpha", "int" },
	{ "text_alpha", "int" },
	{ "damage", "int" },
	{ "hit_by", "null|custom_obj" },

	{ "distortion", "null|object" },
	{ "is_standing", "bool" },
	{ "standing_info", "null|object" },
	
	{ "near_cliff_edge", "bool" },
	{ "distance_to_cliff", "int" },
	
	{ "slope_standing_on", "int" },
	{ "underwater", "bool" },
	
	{ "previous_water_bounds", "[int]" },
	{ "water_bounds", "null|[int]" },
	{ "water_object", "null|custom_obj" },
	
	{ "driver", "null|custom_obj" },
	{ "is_human", "bool" },
	{ "invincible", "bool" },
	
	{ "sound_volume", "int" },
	{ "destroyed", "bool" },
	{ "is_standing_on_platform", "null|bool|custom_obj" },
	{ "standing_on", "null|custom_obj" },
	
	{ "shader", "null|object" },
	{ "effects", "any" },
	{ "variations", "[string]" },
	
	{ "attached_objects", "[custom_obj]" },
	{ "call_stack", "[string]" },
	{ "lights", "[object]" },
	
	{ "solid_dimensions_in", "[int]" },
	{ "solid_dimensions_not_in", "[int]" },
	
	{ "collide_dimensions_in", "[int]" },
	{ "collide_dimensions_not_in", "[int]" },
	
	{ "brightness", "int" },
	{ "current_generator", "object" },
	{ "tags", "object" },
	{ "draw_area", "any" },
	{ "scale", "decimal" },
	
	{ "activation_area", "null|[int]" },
	{ "clip_area", "null|[int]" },

	{ "always_active", "bool" },
	{ "activation_border", "int" },
	{ "fall_through_platforms", "any" },
	{ "has_feet", "bool" },
	
	{ "x_schedule", "any" },
	{ "y_schedule", "any" },
	{ "rotation_schedule", "any" },
	{ "schedule_speed", "any" },
	
	{ "schedule_expires", "any" },
	
	{ "platform_area", "null|[int]" },
	{ "platform_offsets", "[int]" },
	{ "custom_draw", "list" },
	
	{ "uv_array", "[decimal]" },
	{ "xy_array", "[decimal]" },
	{ "uv_segments", "[int]" },
	
	{ "draw_primitives", "[object]" },
	{ "event_handlers", "any" },
	
	{ "use_absolute_screen_coordinates", "bool" },
	
	{ "widgets", "any" },
	{ "textv", "any" },
	{ "body", "any" },
	{ "paused", "any" },
	{ "mouseover_delay", "any" },
	{ "mouseover_area", "any" },
	
	{ "ctrl_up", "any" },
	{ "ctrl_down", "any" },
	{ "ctrl_left", "any" },
	{ "ctrl_right", "any" },
	
	{ "ctrl_attack", "any" },
	{ "ctrl_jump", "any" },
	{ "ctrl_tongue", "any" },
};
	ASSERT_EQ(NUM_CUSTOM_OBJECT_PROPERTIES, sizeof(CustomObjectProperties)/sizeof(*CustomObjectProperties));

	if(global_entries().empty()) {
		for(int n = 0; n != sizeof(CustomObjectProperties)/sizeof(*CustomObjectProperties); ++n) {
			global_entries().push_back(entry(CustomObjectProperties[n].id));
			global_entries().back().set_variant_type(parse_variant_type(variant(CustomObjectProperties[n].type)));
		}

		for(int n = 0; n != global_entries().size(); ++n) {
			keys_to_slots()[global_entries()[n].id] = n;
		}

		global_entries()[CUSTOM_OBJECT_ME].set_variant_type(variant_type::get_custom_object());
		global_entries()[CUSTOM_OBJECT_SELF].set_variant_type(variant_type::get_custom_object());

		const variant_type_ptr builtin = variant_type::get_builtin("level");
		global_entries()[CUSTOM_OBJECT_LEVEL].set_variant_type(builtin);
	}

	global_entries()[CUSTOM_OBJECT_PARENT].type_definition = is_singleton ? this : &instance();
	global_entries()[CUSTOM_OBJECT_LIB].type_definition = game_logic::get_library_definition().get();

	entries_ = global_entries();
}

void custom_object_callable::set_object_type(variant_type_ptr type)
{
	entries_[CUSTOM_OBJECT_ME].set_variant_type(type);
	entries_[CUSTOM_OBJECT_SELF].set_variant_type(type);
}

int custom_object_callable::get_key_slot(const std::string& key)
{
	std::map<std::string, int>::const_iterator itor = keys_to_slots().find(key);
	if(itor == keys_to_slots().end()) {
		return -1;
	}

	return itor->second;
}

int custom_object_callable::get_slot(const std::string& key) const
{
	std::map<std::string, int>::const_iterator itor = properties_.find(key);
	if(itor == properties_.end()) {
		return get_key_slot(key);
	} else {
		return itor->second;
	}
}

game_logic::formula_callable_definition::entry* custom_object_callable::get_entry(int slot)
{
	if(slot < 0 || slot >= entries_.size()) {
		return NULL;
	}

	return &entries_[slot];
}

const game_logic::formula_callable_definition::entry* custom_object_callable::get_entry(int slot) const
{
	if(slot < 0 || slot >= entries_.size()) {
		return NULL;
	}

	return &entries_[slot];
}

void custom_object_callable::add_property(const std::string& id, variant_type_ptr type)
{
	properties_[id] = entries_.size();
	entries_.push_back(entry(id));
	entries_.back().set_variant_type(type);
}
