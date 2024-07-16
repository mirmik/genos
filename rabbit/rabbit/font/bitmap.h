#ifndef RALGO_FONT_BITMAP_H
#define RALGO_FONT_BITMAP_H

namespace ralgo
{
	class bitmap
	{
		int width;
		int height;

		std::vector<uint8_t> data;

	public:
		void resize(int w, int h)
		{
			width = w;
			height = h;
			data.resize(w * h);
		}

		uint8_t& operator ()(int i, int j)
		{
			return data[i * width + j];
		}


		void set_test_bitmap()
		{
			resize(50, 50);
			for (int i = 0; i < 50; ++i)
			{
				for (int j = 0; j < 50; ++j)
				{
					(*this)(i, j) = (i+j) % 2 == 0 ? 255 : 0;
				}
			}
		}
	};
}

#endif