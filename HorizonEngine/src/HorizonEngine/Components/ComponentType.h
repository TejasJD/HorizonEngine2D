#pragma once

#ifndef HZN_COMPONENT_TYPE_H
#define HZN_COMPONENT_TYPE_H

namespace Hzn {
	enum class ComponentType {
		C_Transform,
		C_Collider2D,
		C_BoxCollider2D,
		C_CapsuleCollider2D,
		C_CircleCollider2D,
		C_Rigidbody2D,
		C_CustomComponent
	};
}

#endif