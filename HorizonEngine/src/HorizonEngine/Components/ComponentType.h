#pragma once

#ifndef _component_type_h
#define _component_type_h

namespace Hzn {
	enum class ComponentType {
		C_Transform,
		C_Collider2D,
		C_BoxCollider2D,
		C_CapsuleCollider2D,
		C_CircleCollider2D,
		C_Rigidbody2D
	};
}

#endif