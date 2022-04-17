#pragma once

#ifndef INCLUDE_TYPEINFO_H_
#define INCLUDE_TYPEINFO_H_

#include "classCreateInterface.h"
#include "./classCreateInterface.h"
#include "../utility/TypeDefine.h"
#include "../aegisContainer.h"

#include <vector>
#include <string>
#include <memory>

namespace aegis::typeInfo
{
	using ObjectID = uint64;

	class TypeInfo
	{
	public:
		TypeInfo(TypeInfo* Parent,
			const std::string_view name,
			IClassCreateInterface* createInstanceInterface,
			const bool isAbstract);

		TypeInfo(TypeInfo* Parent,
			const std::string_view name,
			IClassCreateInterface* createInstanceInterface);

		virtual ~TypeInfo() {}

		inline ObjectID GetTypeID() const { return mTypeID; }
		inline ObjectID GetTypeSize() const { return 0; }
		inline std::string_view GetTypeName() const { return mName; }

		void* CreateInstance() const
		{
			if (mCreateInstanceInterface)
				return mCreateInstanceInterface->CreateInstance();
			else
				return nullptr;
		}

		inline aegis::vector<TypeInfo*>* GetChildren() { return &mChildren;  }


		inline bool IsAbstract() { return mIsAbstract; }

		bool IsSame(TypeInfo* typeInfo) const;

		bool IsSameAbstract(TypeInfo* typeInfo) const;

		bool operator==(TypeInfo* typeInfo) const;
		bool operator!=(TypeInfo* typeInfo) const;

	protected:

	private:

		bool mIsAbstract;
		ObjectID mTypeID;
		std::string mName;
		TypeInfo* mParent;
		std::unique_ptr<IClassCreateInterface> mCreateInstanceInterface;
		aegis::vector<TypeInfo*> mChildren;
	};

	inline bool TypeInfo::operator==(TypeInfo* typeInfo) const
	{
		return IsSame(typeInfo);
	}
	
	inline bool TypeInfo::operator!=(TypeInfo* typeInfo) const
	{
		return !IsSame(typeInfo);
	}
}

#endif // !INCLUDE_TYPEINFO_H_
