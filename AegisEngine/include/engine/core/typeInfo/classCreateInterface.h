#pragma once

#ifndef INCLUDE_ICLASSCREATEINTERFACE_H_
#define INCLUDE_ICLASSCREATEINTERFACE_H_

namespace aegis::typeInfo
{
	class IClassCreateInterface
	{
	public:
		IClassCreateInterface() = default;
		virtual ~IClassCreateInterface() {}

		virtual void* CreateInstance() { return nullptr; }
	};

	template<class T>
	class IClassCreater : public IClassCreateInterface
	{
	public:
		IClassCreater() = default;
		~IClassCreater() {}

		void* CreateInstance() override { return new T(); }
	};
}

#endif // !INCLUDE_ICLASSCREATEINTERFACE_H_
