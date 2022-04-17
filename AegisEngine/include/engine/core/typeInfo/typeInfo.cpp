#include "typeInfo.h"
#include "../hash/hash.h"

namespace aegis::typeInfo
{
	TypeInfo::TypeInfo(TypeInfo* Parent,
		const std::string_view name,
		IClassCreateInterface* createInstanceInterface,
		const bool isAbstract)
	{
		mIsAbstract = isAbstract;
		mTypeID = HashCRC32(name);
		mName = name;
		mCreateInstanceInterface.reset(createInstanceInterface);

		mParent = Parent;

		if (nullptr != mParent)
			mParent->mChildren.emplace_back(this);

		//DynamicTypeManager::setDynamicType(this->GetTypeID(), mCreateInstanceInterface.get());
	}

	TypeInfo::TypeInfo(TypeInfo* Parent,
		const std::string_view name,
		IClassCreateInterface* createInstanceInterface)
	{
		mIsAbstract = false;
		mTypeID = HashCRC32(name);
		mName = name;
		mCreateInstanceInterface.reset(createInstanceInterface);

		mParent = Parent;

		if (nullptr != mParent)
			mParent->mChildren.emplace_back(this);

		//DynamicTypeManager::setDynamicType(this->GetTypeID(), mCreateInstanceInterface.get());
	}

	bool TypeInfo::IsSame(TypeInfo* typeInfo) const
	{
		if (nullptr != typeInfo)
		{
			return this->GetTypeID() == typeInfo->GetTypeID();
		}
		return false;
	}

	bool TypeInfo::IsSameAbstract(TypeInfo* typeInfo) const
	{
		if (nullptr != typeInfo)
		{
			if (this->GetTypeID() == typeInfo->GetTypeID())
			{
				return true;
			}
			else
			{
				if (this->mParent != nullptr)
				{
					if (this->mParent->GetTypeID() == typeInfo->GetTypeID())
					{
						return true;
					}
					else
					{
						for (const auto& child : mChildren)
						{
							if (child->GetTypeID() == typeInfo->GetTypeID())
							{
								return true;
							}
						}

						return false;
					}
				}
				else
				{
					return false;
				}
			}


			//if (this->GetTypeID() != typeInfo->GetTypeID()
			//	&& nullptr != this->mParent
			//	&& nullptr != typeInfo->mParent)
			//{
			//	return this->mParent->IsSameAbstract(typeInfo->mParent);
			//}
			//return false;
		}
		return false;
	}
}
