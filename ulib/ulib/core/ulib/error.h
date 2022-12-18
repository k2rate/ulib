#pragma once

#include "types.h"
#include <assert.h>

namespace ulib
{
	class Error
	{
	public:

		Error() : c(Error::Success)
		{}

		Error(uint32 t) : c(t)
		{}

		enum : uint32
		{
			Success = 0,

			// filesystem
			FileNotFound = 1,

			// networking
			AddressFamilyNotSupported = 200,		
			ProtocolIsNotSupported,
			SocketTypeIsNotSupported,		
			ProtocolIsNotCompatibleWithSocketType,
			SocketIsNotAvailable,
			AddressAlreadyUse,
			InvalidAddress,
			NotEnoughPorts,
			AlreadyRunning,
			AlreadyConnected,
			ConnectionRefused,


			Unknown = 0x1000
		};

		inline Error& operator=(Error e) { c = e.c; return *this; }
		inline bool operator==(Error e) const { return c == e.c; }
		inline bool operator!=(Error e) const { return c != e.c; }

		inline Error& operator=(uint32 e) { c = e; return *this; }
		inline bool operator==(uint32 e) const { return c == e; }
		inline bool operator!=(uint32 e) const { return c != e; }

		inline operator bool() const { return c != Error::Success; }

		inline uint32 Code() const { return c; }

	private:
		uint32 c;
	};
}