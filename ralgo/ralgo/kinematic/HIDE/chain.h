#ifndef RALGO_CYNEMATIC_CHAIN_H
#define RALGO_CYNEMATIC_CHAIN_H

/**
    @file
*/

#include <ralgo/linalg/bivec.h>
#include <ralgo/linalg/htrans.h>
#include <ralgo/linalg/optimize.h>

namespace ralgo
{
    namespace cynematic
    {
        template <class T>
        bivec<T, 3> speed_trans(const bivec<T, 3> &bispeed,
                                const htrans<T> &link)
        {
            auto w = bispeed.a;
            auto v = bispeed.b;

            auto trmat = link.inverse();

            auto res_w = trmat.transform_vector(w);
            auto res_v = trmat.transform_vector(cross(w, link.center) + v);

            return {res_w, res_v};
        }

        template <typename T> struct chain
        {

        private:
            std::vector<abstract_link<T> *> links;
            size_t coords_total = 0;

        public:
            chain() {}

            chain(std::initializer_list<abstract_link<T> *> lst) : links(lst)
            {
                for (auto *r : links)
                {
                    coords_total += r->count_of_coords();
                }
            }

            chain(std::vector<abstract_link<T> *> lst) : links(lst)
            {
                for (auto *r : links)
                {
                    coords_total += r->count_of_coords();
                }
            }

            void add_link(abstract_link<T> *lnk)
            {
                links.push_back(lnk);
                coords_total += lnk->count_of_coords();
            }

            htrans<T> get(const std::vector<T> &coords)
            {
                htrans<T> result{};
                malgo::vector<T> cccc(coords.data(), coords.size());
                int8_t coord_pos = coords.size() - 1;

                for (int i = links.size() - 1; i >= 0; --i)
                {
                    uint8_t count_of_coords = links[i]->count_of_coords();

                    if (coord_pos - count_of_coords + 1 < 0)
                        return htrans<T>();

                    htrans<T> nmat = links[i]->get(cccc, coord_pos);
                    result = nmat * result;
                    coord_pos -= count_of_coords;
                }

                return result;
            }

            htrans<T> get(const malgo::vector<T> &coords)
            {
                htrans<T> result{};
                // malgo::vector<T> cccc(coords.data(), coords.size());
                int8_t coord_pos = coords.size() - 1;

                for (int i = links.size() - 1; i >= 0; --i)
                {
                    uint8_t count_of_coords = links[i]->count_of_coords();

                    if (coord_pos - count_of_coords + 1 < 0)
                        return htrans<T>();

                    htrans<T> nmat = links[i]->get(coords, coord_pos);
                    result = nmat * result;
                    coord_pos -= count_of_coords;
                }

                return result;
            }

            std::vector<vec<T, 6>>
            get_speed_transes(const malgo::vector<T> &coords)
            {
                std::vector<vec<T, 6>> result;

                htrans<T> curtrans{};
                int8_t coord_pos = coords.size() - 1;

                for (int i = links.size() - 1; i >= 0; --i)
                {
                    uint8_t count_of_coords = links[i]->count_of_coords();

                    //Проверка корректности вектора координат.
                    if (coord_pos - count_of_coords + 1 < 0)
                        return std::vector<vec<T, 6>>();

                    if (count_of_coords > 0)
                    {
                        if (count_of_coords == 1)
                        {
                            result.emplace_back(
                                speed_trans(
                                    ((onedof_link<T> *)links[i])->d1_bivec(),
                                    curtrans)
                                    .concat());
                        }
                        else
                        {
                            BUG();
                        }
                    }

                    //перематываем на следующий линк.
                    htrans<T> nmat = links[i]->get(coords, coord_pos);
                    curtrans = nmat * curtrans;

                    coord_pos -= count_of_coords;
                }

                std::reverse(result.begin(), result.end());
                return result;
            }

            bivec<T, 3> get_sens(int idx, const malgo::vector<T> &coords)
            {
                uint8_t coordpos = coords_total - 1;
                htrans<T> curtrans{};

                int linkidx = links.size() - 1;

                while (coordpos != idx)
                {
                    uint8_t count_of_coords = links[linkidx]->count_of_coords();
                    auto nmat = links[linkidx]->get(coords, coordpos);

                    curtrans = nmat * curtrans;
                    coordpos -= count_of_coords;
                    linkidx--;
                }

                return speed_trans(
                    ((onedof_link<T> *)links[linkidx])->d1_bivec(), curtrans);
            }

            bivec<T, 3> get_sens_base(int idx, const malgo::vector<T> &coords)
            {
                uint8_t coordpos = coords_total - 1;
                htrans<T> curtrans{};

                int linkidx = links.size() - 1;

                while (coordpos != idx)
                {
                    uint8_t count_of_coords = links[linkidx]->count_of_coords();
                    auto nmat = links[linkidx]->get(coords, coordpos);
                    curtrans = nmat * curtrans;
                    coordpos -= count_of_coords;
                    linkidx--;
                }

                auto bi = ((onedof_link<T> *)links[linkidx])->d1_bivec();
                auto w = bi.a;
                auto v = cross(w, curtrans.center) + bi.b;

                htrans<T> basetrans{};

                while (linkidx >= 0)
                {
                    uint8_t count_of_coords = links[linkidx]->count_of_coords();
                    auto nmat = links[linkidx]->get(coords, coordpos);
                    basetrans = nmat * basetrans;
                    coordpos -= count_of_coords;
                    linkidx--;
                }

                return {basetrans.rotate(w), basetrans.rotate(v)};
            }

            std::pair<std::vector<T>, int>
            solve_inverse_cynematic(const htrans<T> &target,
                                    const std::vector<T> &_reference,
                                    T maxstep = 5)
            {
                assert(_reference.size() == coords_total);
                malgo::vector<T> reference(_reference.data(),
                                           _reference.size());

                htrans<T> itr;
                // T lastlen = std::numeric_limits<T>::max();

                int iteration;
                for (iteration = 0; iteration < 1000; ++iteration)
                {
                    auto curtrans = get(reference);

                    auto rrr = get_speed_transes(reference);
                    auto iv6 = curtrans.vector6_to(target);

                    // std::cout<<iv6 <<" "<<pseudolen1(iv6)<<std::endl;
                    if (linalg::pseudolen1(iv6) < 0.000001)
                    {
                        nos::println("RESULT:", reference);
                        break;
                    }

                    auto koeffs = vecops::backpack(iv6, rrr);

                    for (auto &k : koeffs)
                        k = k * 1.05;

                    if (koeffs.size() == 0)
                        return {};

                    T koeffs_length = length(koeffs);
                    // for (auto f : koeffs) koeffs_length += f * f;
                    // koeffs_length = sqrt(koeffs_length);///=
                    // (T)koeffs.size(); for (auto f : koeffs) koeffs_length +=
                    // abs(f); koeffs_length /= koeffs.size();

                    nos::println();

                    // nos::readline();*/

                    for (unsigned int i = 0; i < coords_total; ++i)
                    {
                        reference[i] +=
                            koeffs[i] * (koeffs_length > maxstep
                                             ? (maxstep / koeffs_length)
                                             : 1);
                    }
                }

                std::vector<T> rreference(reference.begin(), reference.end());
                return {rreference, iteration};
            }
        };
    }
}

#endif
