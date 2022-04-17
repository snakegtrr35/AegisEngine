#pragma once

#ifndef INCLUDE_AEGISOBJECT_H_
#define INCLUDE_AEGISOBJECT_H_

#include "typeInfo\DTI.h"

namespace aegis
{
	class AegisObject
	{
		BASE_OBJECT_TYPE_INFO(AegisObject)

	public:
		virtual ~AegisObject() {}
	};
}

#endif // !INCLUDE_AEGISOBJECT_H_