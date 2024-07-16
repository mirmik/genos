#include <doctest/doctest.h>
#include <fcntl.h>
#include <ndmath/util.h>
#include <nos/fprint.h>
#include <nos/input.h>
#include <nos/io/file.h>
#include <nos/trent/json_print.h>

ndmath::cartesian_correction
one_axis_cartesian_correction::arrays_to_correction(std::vector<double> _points,
                                                    std::vector<double> _deltas)
{
    std::vector<double> points;
    std::vector<double> deltas;

    std::vector<std::pair<double, double>> arrays;
    for (size_t i = 0; i < _points.size(); i++)
    {
        arrays.push_back({_points[i], _deltas[i]});
    }
    std::sort(arrays.begin(), arrays.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });
    for (size_t i = 0; i < _points.size(); i++)
    {
        points.push_back(arrays[i].first);
        deltas.push_back(arrays[i].second);
    }

    igris::ndarray<ndmath::vector> darr;
    darr.resize(std::vector<size_t>{(size_t)deltas.size()});

    for (size_t i = 0; i < deltas.size(); ++i)
    {
        darr({i}) = ndmath::vector{deltas[i]};
    }

    ndmath::cartesian_correction cc;
    cc.set_grid({points});
    cc.set_deltas(darr);
    assert(cc.dim() == 1);
    return cc;
}

std::pair<std::vector<double>, std::vector<double>>
one_axis_cartesian_correction::correction_to_arrays(
    const ndmath::cartesian_correction &cc)
{
    if (cc.grid().get_coords().size() == 0)
        return {{}, {}};
    std::vector<double> points = cc.grid().get_coords()[0];

    std::vector<double> deltas;
    for (size_t i = 0; i < points.size(); ++i)
    {
        deltas.push_back(cc.deltas()({i})[0]);
    }

    return {points, deltas};
}

ndmath::cartesian_correction
one_axis_cartesian_correction::trent_to_correction(const nos::trent &table)
{
    if (!table.contains("points") || table["points"].as_list().size() == 0)
        return one_axis_cartesian_correction::arrays_to_correction({}, {});
        
    if (!table.contains("deltas") || table["deltas"].as_list().size() != table["points"].as_list().size())
        return one_axis_cartesian_correction::arrays_to_correction({}, {});

    auto points = ralgo::vecops::transform<std::vector<double>>(
        table["points"].as_list(), [](auto &p) { return p.as_numer(); });

    auto deltas = ralgo::vecops::transform<std::vector<double>>(
        table["deltas"].as_list(), [](auto &p) { return p.as_numer(); });

    return arrays_to_correction(points, deltas);
}

nos::trent one_axis_cartesian_correction::correction_to_trent(
    const ndmath::cartesian_correction &cc)
{
    auto [points, deltas] = correction_to_arrays(cc);

    nos::trent tr;
    tr["points"].as_list();
    tr["deltas"].as_list();

    for (auto &p : points)
        tr["points"].as_list().push_back(p);

    for (auto &p : deltas)
        tr["deltas"].as_list().push_back(p);

    return tr;
}

nos::trent multi_axes_cartesian_correction::correction_to_trent(
    const ndmath::cartesian_correction &cc)
{
    auto grid = cc.grid();
    auto deltas_shape = cc.deltas().shape();
    auto deltas_storage = cc.deltas().storage();

    nos::trent tr;
    tr["grid"].as_list();
    tr["deltas"].as_dict();

    for (size_t i = 0; i < grid.dim(); ++i)
    {
        auto current_dim_coords = grid.get_coords()[i];
        auto &ctr = tr["grid"][i].as_list();
        for (auto &p : current_dim_coords)
            ctr.push_back(p);
    }

    tr["deltas"]["shape"].as_list();
    for (auto &p : deltas_shape)
        tr["deltas"]["shape"].as_list().push_back(p);

    tr["deltas"]["storage"].as_list();
    for (size_t i = 0; i < deltas_storage.size(); ++i)
    {
        auto &ctr = tr["deltas"]["storage"][i].as_list();
        for (auto &p : deltas_storage[i])
            ctr.push_back(p);
    }

    return tr;
}

ndmath::cartesian_correction
multi_axes_cartesian_correction::trent_to_correction(const nos::trent &table)
{
    auto &tr_grid = table["grid"].as_list();
    auto &tr_deltas_shape = table["deltas"]["shape"].as_list();
    auto &tr_deltas_storage = table["deltas"]["storage"].as_list();

    std::vector<std::vector<double>> grid;
    std::vector<size_t> deltas_shape;
    std::vector<ndmath::vector> deltas_storage;

    for (auto &p : tr_grid)
    {
        grid.emplace_back();
        auto &ctr = grid.back();
        auto &tr_ctr = p.as_list();
        for (auto &p : tr_ctr)
            ctr.push_back(p.as_numer());
    }

    for (auto &p : tr_deltas_shape)
        deltas_shape.push_back(p.as_numer());

    for (auto &p : tr_deltas_storage)
    {
        deltas_storage.emplace_back();
        auto &ctr = deltas_storage.back();
        auto &tr_ctr = p.as_list();
        for (auto &p : tr_ctr)
            ctr.push_back(p.as_numer());
    }

    ndmath::cartesian_correction cc;
    cc.set_grid(grid);
    igris::ndarray<ndmath::vector> darr(deltas_storage, deltas_shape);
    cc.set_deltas(darr);

    return cc;
}

ndmath::cartesian_correction
one_axis_cartesian_correction::correction_from_csv_file(std::string fname)
{
    ndmath::cartesian_correction cc;
    auto file = nos::file(fname.c_str(), O_RDONLY);
    std::vector<double> grid;
    std::vector<double> values;
    std::vector<double> deltas;

    while (true)
    {
        auto errstr = nos::readline_from(file);
        if (errstr.is_error() || errstr->size() == 0)
            break;
        auto str = *errstr;
        auto lst = nos::split(str, ',');
        double el_grid = std::stod(nos::trim(lst[0]));
        double el_value = std::stod(nos::trim(lst[1]));
        grid.push_back(el_grid);
        values.push_back(el_value);
        deltas.push_back(el_grid - el_value);
    }

    igris::ndarray<ndmath::vector> darr;
    darr.resize(std::vector<size_t>{(size_t)deltas.size()});
    for (size_t i = 0; i < deltas.size(); ++i)
    {
        darr({i}) = ndmath::vector{deltas[i]};
    }

    cc.set_grid({grid});
    cc.set_deltas(darr);
    return cc;
}

TEST_CASE("cartesian_correction.to_trent")
{
    ndmath::cartesian_correction cartesian_correction;
    cartesian_correction.set_grid({{0}, {-10, 0, 10}, {0}});
    cartesian_correction.set_deltas(igris::ndarray<ndmath::vector>(
        {{0, 0, 0.1}, {0, 0, 0.2}, {0, 0, 0.3}}, {1, 3, 1}));

    auto trent = multi_axes_cartesian_correction::correction_to_trent(
        cartesian_correction);

    auto str = nos::format("{}", trent);
    CHECK_EQ(
        str,
        R"({"grid":[[0.00000],[-10.00000,0.00000,10.00000],[0.00000]],"deltas":{"shape":[1.00000,3.00000,1.00000],"storage":[[0.00000,0.00000,0.10000],[0.00000,0.00000,0.20000],[0.00000,0.00000,0.30000]]}})");

    auto cc = multi_axes_cartesian_correction::trent_to_correction(trent);
    CHECK_EQ(cc.grid().dim(), 3);
    CHECK_EQ(cc.grid().get_coords()[0], std::vector<double>{0});
    CHECK_EQ(cc.grid().get_coords()[1], std::vector<double>{-10, 0, 10});
    CHECK_EQ(cc.grid().get_coords()[2], std::vector<double>{0});
    CHECK_EQ(cc.deltas().shape(), std::vector<size_t>{1, 3, 1});
}

TEST_CASE("one_axis_correction converters")
{
    std::vector<double> points = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> deltas = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto cc =
        one_axis_cartesian_correction::arrays_to_correction(points, deltas);

    CHECK_EQ(cc.grid().get_coords()[0], points);
    CHECK_EQ(cc.deltas()({0})[0], 0);
}

TEST_CASE("one_axis_correction trent")
{
    std::vector<double> points = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> deltas = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto cc =
        one_axis_cartesian_correction::arrays_to_correction(points, deltas);

    auto tr = one_axis_cartesian_correction::correction_to_trent(cc);
    auto cc2 = one_axis_cartesian_correction::trent_to_correction(tr);

    CHECK_EQ(cc.grid().get_coords()[0], cc2.grid().get_coords()[0]);
    CHECK_EQ(cc.deltas()({0})[0], cc2.deltas()({0})[0]);

    auto tr_str = nos::json::to_string(tr, false);

    CHECK_EQ(
        tr_str,
        R"({"points":[0,1,2,3,4,5,6,7,8,9],"deltas":[0,1,2,3,4,5,6,7,8,9]})");
}
