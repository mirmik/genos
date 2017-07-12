#ifndef GENOS_FILE_H
#define GENOS_FILE_H

namespace Genos {
	//enum FileFlags {
	//	Locked = 0x0001;
	//}

	class File {
	protected:
		uint16_t flags;

	public:
		int32_t write(const char* data, size_t maxsz, uint16_t flags);
		int32_t read(char* data, size_t maxsz, uint16_t flags);
		int32_t seek(int32_t pos, uint16_t flag);

		virtual int32_t avail() { return -1; } 
		virtual int32_t room() { return -1; }

		virtual int32_t open(uint16_t flags) = 0;
		virtual int32_t release() = 0;

	protected:
		virtual int32_t writeData(const char* data, size_t maxsz, size_t pos) = 0;
		virtual int32_t readData(char* data, size_t maxsz, size_t pos) = 0;
	};
}

#endif