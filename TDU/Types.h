#pragma once
#include <cstring>
#include <stdlib.h>
#include <cstdlib>

// These were made by SK83RJOSH

#define MemoryAlloc &malloc;
#define MemoryFree &free;

template<class T>
T* MemoryNew()
{
	return (T*)&MemoryAlloc(sizeof(T));
}

template<class T>
T* MemoryNew(size_t size)
{
	T* result;
	result = (T*)MemoryAlloc(sizeof(T) * size);
	return result;
//	return *(T**)(MemoryAlloc(sizeof(T) * size));
}

class small_string
{
public:
	small_string() {};
	small_string(const char* str) {
		size_t len = strlen(str);
		char* dst = m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0];

		if (len > 15)
		{
			dst = MemoryNew<char>(len + 1);

			if (dst == nullptr)
			{
				return;
			}

			if (m_StackBuffer[15])
			{
				MemoryFree(m_HeapBuffer);
			}
			else
			{
				m_StackBuffer[15] = 1;
			}

			m_HeapBuffer = dst;
		}

		memcpy(dst, str, len);
		dst[len] = 0;
	}
	small_string(const small_string&) = delete;
	//void operator=(const small_string&) = delete;
	~small_string() {
		if (m_StackBuffer[15])
		{
			MemoryFree(m_HeapBuffer);
		}
	}

	const char* c_str() const { return m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0]; }

private:
	union {
		char* m_HeapBuffer;
		char m_StackBuffer[16] = { 0 };
	};
};
