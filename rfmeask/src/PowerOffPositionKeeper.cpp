#include <PowerOffPositionKeeper.h>
#include <logging.h>
#include <nos/fprint.h>

PowerOffPositionKeeper::PowerOffPositionKeeper(const std::string_view &str,
                                               double period) :
    path(std::string(str)), period(period), sector_width(period / parts)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    invalidate();
    stored_current_sector = restore_sector_from_storage(path);
    nos::log::debug(
        "PowerOffPositionKeeper str:{}, period:{}, stored_current_sector:{}",
        str,
        period,
        stored_current_sector);
}

bool PowerOffPositionKeeper::is_inited() const
{
    return inited;
}

double PowerOffPositionKeeper::process_position(
    const double current_non_corrected_position)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);

    if (!inited)
        nos::log::warn("PowerOffPositionKeeper is not inited for now");

    const double &curpos = current_non_corrected_position;
    double cursec = sectorno_of_position(curpos);

    double corrected_position =
        non_corrected_position_to_corrected_position(curpos);
    int corrected_sector = non_corrected_sector_to_corrected_sector(cursec);

    double hi = high_trigger_level_of_sector(stored_current_sector);
    double lo = low_trigger_level_of_sector(stored_current_sector);

    if (corrected_position > hi || corrected_position < lo)
    {
        if (current_non_corrected_position != 0)
        {
            if (std::abs(corrected_sector - stored_current_sector) == 1)
                update_stored_current_sector(corrected_sector);
            else
            {
                nos::log::debug(
                    "update_stored_current_sector "
                    "(corrected_sector:{}) prevented because "
                    "its difference with stored_current_sector is not one",
                    corrected_sector);
            }
        }
        else
            nos::log::debug("update_stored_current_sector "
                            "(corrected_sector:{}) prevented because "
                            "current_non_corrected_position is zero",
                            corrected_sector);
    }

    return corrected_position;
}

double PowerOffPositionKeeper::half_period() const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return period / 2;
}

double PowerOffPositionKeeper::half_sector_width() const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return sector_width / 2;
}

int PowerOffPositionKeeper::restore_sector_from_storage(
    const std::filesystem::path &path)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    // make directory if not exists
    std::filesystem::create_directories(path.parent_path());

    // is file exists?
    if (!std::filesystem::exists(path))
    {
        // create file
        std::ofstream ofs(path, std::ios::binary);
        ofs << 0;
    }

    // read file
    std::ifstream ifs(path, std::ios::binary);
    int sector;
    ifs >> sector;
    return sector;
}

int PowerOffPositionKeeper::compensation_counter_value() const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return compensation_counter;
}

int PowerOffPositionKeeper::current_sector() const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return stored_current_sector;
}

void PowerOffPositionKeeper::invalidate()
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    inited = false;
    compensation_counter = 0;
}

void PowerOffPositionKeeper::reinit_with_start_position(double start_position)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    invalidate();
    stored_current_sector = restore_sector_from_storage(path);

    double center_of_restored_sector =
        stored_current_sector * sector_width + half_sector_width();

    while (true)
    {
        double delta = start_position - center_of_restored_sector +
                       compensation_counter * period;
        if (delta > half_period())
            compensation_counter--;
        else if (delta < -half_period())
            compensation_counter++;
        else
            break;
    }

    nos::log::debug("PowerOffPositionKeeper reinit_with_start_position: {} "
                    "storred_sector: {}, compensation_counter: {}",
                    start_position,
                    stored_current_sector,
                    compensation_counter);

    inited = true;
}

void PowerOffPositionKeeper::store_sector(int sector)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    if (!inited)
    {
        nos::log::debug(
            "PowerOffPositionKeeper try to storing sector while noninited");
        return;
    }

    nos::log::debug("PowerOffPositionKeeper store sector: {}", sector);
    std::ofstream ofs(path, std::ios::binary);
    ofs << sector;
}

void PowerOffPositionKeeper::update_stored_current_sector(int sectorno)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    if (!inited)
    {
        nos::log::debug("PowerOffPositionKeeper try to update storring sector "
                        "while noninited");
        return;
    }

    stored_current_sector = sectorno;
    store_sector(sectorno);
}

double PowerOffPositionKeeper::non_corrected_position_to_corrected_position(
    double arg) const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return arg + compensation_counter * period;
}

int PowerOffPositionKeeper::non_corrected_sector_to_corrected_sector(
    int arg) const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return arg + compensation_counter * parts;
}

double PowerOffPositionKeeper::high_trigger_level_of_sector(int sectorno) const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return (sectorno + 1) * sector_width + sector_width * trigger_level;
}

double PowerOffPositionKeeper::low_trigger_level_of_sector(int sectorno) const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return sectorno * sector_width - sector_width * trigger_level;
}

int PowerOffPositionKeeper::sectorno_of_position(double position) const
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return std::floor(position / sector_width);
}