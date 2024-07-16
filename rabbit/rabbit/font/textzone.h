#ifndef RABBIT_TEXTZONE_H
#define RABBIT_TEXTZONE_H

#include <ralgo/linspace.h>
#include <rabbit/cell3d.h>

namespace rabbit
{

	class textzone
	{
		int _width;
		int _height;

		linalg::vec<float, 3> ll = { -1, -1, 0.99999};
		linalg::vec<float, 3> hl = { 1, -1, 0.99999};
		linalg::vec<float, 3> lh = { -1,  1, 0.99999};
		linalg::vec<float, 3> hh = { 1,  1, 0.99999};

	public:
		int width() { return _width; }
		int height() { return _height; }

		textzone() = default;
		textzone(int w, int h) : _width(w), _height(h) {}
		textzone(
		    int w, int h,
		    const linalg::vec<float, 3> & ll,
		    const linalg::vec<float, 3> & hl,
		    const linalg::vec<float, 3> & lh,
		    const linalg::vec<float, 3> & hh
		) : _width(w), _height(h), ll(ll), hl(hl), lh(lh), hh(hh)
		{}

		void init(int w, int h) 
		{
			_width = w;
			_height = h;
		}

		void bind_coords(
		    linalg::vec<float, 3> ll,
		    linalg::vec<float, 3> hl,
		    linalg::vec<float, 3> lh,
		    linalg::vec<float, 3> hh)
		{
			this->ll = ll;
			this->hl = hl;
			this->lh = lh;
			this->hh = hh;
		}

		cell3d zone_for_cell(
		    int column,
		    int row)
		{
			cell3d cell;
			row = _height - row - 1;

			float rkoeff1 = (float)row / (float)(_height);
			float ckoeff1 = (float)column / (float)(_width);
			float rkoeff2 = (float)(row + 1) / (float)(_height);
			float ckoeff2 = (float)(column + 1) / (float)(_width);

			cell.ll = ralgo::bilinear_interpolation(ckoeff1, rkoeff1, ll, hl, lh, hh);
			cell.hl = ralgo::bilinear_interpolation(ckoeff2, rkoeff1, ll, hl, lh, hh);
			cell.lh = ralgo::bilinear_interpolation(ckoeff1, rkoeff2, ll, hl, lh, hh);
			cell.hh = ralgo::bilinear_interpolation(ckoeff2, rkoeff2, ll, hl, lh, hh);

			return cell;
		}

		cell3d operator()(int i, int j) { return zone_for_cell(i, j); }
	};

	class textzone_cursor
	{
		int col = 0;
		int row = 0;
		textzone * zone;

	public:
		textzone_cursor(textzone * zone, int col = 0, int row = 0) :
			zone(zone), col(col), row(row) {}

		void increment()
		{
			col++;
			if (col >= zone->width())
			{
				col = 0;
				row++;
			}
		}

		cell3d get() { return zone->zone_for_cell(col, row); }
	};
}

#endif