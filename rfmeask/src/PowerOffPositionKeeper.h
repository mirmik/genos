#ifndef POWER_OFF_POSITION_KEEPER_H
#define POWER_OFF_POSITION_KEEPER_H

#include <cmath>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

/**
 * Sectors for circle periodical function:
 *                           0
 *                -1  ___.___
 *                 _--   |   --_
 *                / ._   |   _.  \
 *            -2 |     . | .     | 1
 *               |    _. | ._    |
 *                \ .    |    . /
 *                  -_   |   _-
 *                 -3  --.--     2
 *
 * Алгоритм запоминает сектор, в котором объект находился в момент отключения
 * при загрузке запрошенная позиция корректируется путём добавления и вычитания
 * периодов таким образом, чтобы позиция отличалась от центра восстановленного
 * сектора не более чем на половину периода.
 * Для определения необходимости обновления записи в хранилище используется
 * логика триггера Шмидта (см. https://ru.wikipedia.org/wiki/Триггер_Шмидта).
 * Данные сохраняются в специальном файле, связанном с именем объекта.
 * Всего в алгоритме используется 4 попарно связанных множества:
 * 1. некоректированная положение NP
 * 2. некоректированный сектор NS
 * 3. коректированная положение CP
 * 4. коректированный сектор CS
 * отображение NP -> CP: c = n + v * period
 * отображение NS -> CS: c = n + v * parts
 * , где v - счётчик периодов коррекции, вычисляется при пресете корректируемого
 * объекта.
 */

class PowerOffPositionKeeper
{
    std::string path = {};
    double period;
    double sector_width;
    int compensation_counter = 0;
    int stored_current_sector = 0;
    bool inited = false;
    static constexpr int parts = 6;
    static constexpr double trigger_level = 0.05;
    mutable std::recursive_mutex mtx = {};

public:
    PowerOffPositionKeeper(const std::string_view &str, double period);
    PowerOffPositionKeeper(const PowerOffPositionKeeper &oth) = default;
    PowerOffPositionKeeper(PowerOffPositionKeeper &&oth) = default;
    PowerOffPositionKeeper &
    operator=(const PowerOffPositionKeeper &oth) = default;
    PowerOffPositionKeeper &operator=(PowerOffPositionKeeper &&oth) = default;
    bool is_inited() const;
    double process_position(const double current_non_corrected_position);
    void reinit_with_start_position(double start_position);
    int sectorno_of_position(double position) const;
    int compensation_counter_value() const;
    int current_sector() const;
    void update_stored_current_sector(int sectorno);

private:
    double half_period() const;
    double half_sector_width() const;
    int restore_sector_from_storage(const std::filesystem::path &path);
    void invalidate();
    void store_sector(int sector);
    double non_corrected_position_to_corrected_position(double arg) const;
    int non_corrected_sector_to_corrected_sector(int arg) const;
    double high_trigger_level_of_sector(int sectorno) const;
    double low_trigger_level_of_sector(int sectorno) const;
};

#endif