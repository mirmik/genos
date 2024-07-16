#include <ralgo/clinalg/matops.h>
#include <ralgo/heimer/axstate_linear_processor.h>
#include <ralgo/heimer/sigtypes.h>

#include <ralgo/linalg/matops.h>
#include <ralgo/log.h>

#include <igris/util/numconvert.h>

using namespace heimer;

int axstate_linear_processor::serve(disctime_t)
{
    for (int i = 0; i < dim(); ++i)
    {
        position_t accpos = 0;
        velocity_t accvel = 0;
        for (int j = 0; j < dim(); ++j)
        {
            accpos += *(matrix + i * dim() + j) * rightax(j)->ctrpos;
            accvel += *(matrix + i * dim() + j) * rightax(j)->ctrvel;
        }

        leftax(i)->ctrpos = accpos;
        leftax(i)->ctrvel = accvel;
    }

    return 0;
}

int axstate_linear_processor::feedback(disctime_t)
{
    for (int i = 0; i < dim(); ++i)
    {
        position_t accpos = 0;
        velocity_t accvel = 0;
        for (int j = 0; j < dim(); ++j)
        {
            accpos += *(invert_matrix + i * dim() + j) * leftax(j)->feedpos;
            accvel += *(invert_matrix + i * dim() + j) * leftax(j)->feedvel;
        }

        rightax(i)->feedpos = accpos;
        rightax(i)->feedvel = accvel;
    }

    return 0;
}

static inline int matrix(axstate_linear_processor *axctr,
                         int argc,
                         char **argv,
                         char *output,
                         int outmax)
{
    if (argc != axctr->dim() * axctr->dim())
    {
        snprintf(output, outmax, "Wrong matrix size");
        return -1;
    }

    for (int i = 0; i < argc; ++i)
    {
        axctr->matrix[i] = atof(argv[i]);
    }

    axctr->evaluate_invertion();

    return 0;
}

static inline int info(axstate_linear_processor *axctr,
                       int argc,
                       char **argv,
                       char *output,
                       int outmax)
{
    (void)argc;
    (void)argv;

    const int bufsize = 48;
    char buf[bufsize];

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "lsignals: ");
    strncat(output, buf, outmax);

    for (int i = 0; i < axctr->dim() - 1; ++i)
    {
        memset(buf, 0, bufsize);
        snprintf(buf, bufsize, "%s,", axctr->leftax(i)->name.c_str());
        strncat(output, buf, outmax);
    }
    memset(buf, 0, bufsize);
    snprintf(
        buf, bufsize, "%s\r\n", axctr->leftax(axctr->dim() - 1)->name.c_str());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "rsignals: ");
    strncat(output, buf, outmax);

    for (int i = 0; i < axctr->dim() - 1; ++i)
    {
        memset(buf, 0, bufsize);
        snprintf(buf, bufsize, "%s,", axctr->rightax(i)->name.c_str());
        strncat(output, buf, outmax);
    }

    memset(buf, 0, bufsize);
    snprintf(
        buf, bufsize, "%s\r\n", axctr->rightax(axctr->dim() - 1)->name.c_str());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "matrix: ");
    strncat(output, buf, outmax);
    for (int i = 0; i < axctr->dim() * axctr->dim(); ++i)
    {
        memset(buf, 0, bufsize);
        igris_f32toa(axctr->matrix[i], buf, 5);
        // snprintf(buf, bufsize, "%f ", axctr->matrix[i]);
        strncat(output, buf, outmax);
        strncat(output, " ", outmax);
    }
    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "\r\n");
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "invmat: ");
    strncat(output, buf, outmax);
    for (int i = 0; i < axctr->dim() * axctr->dim(); ++i)
    {
        memset(buf, 0, bufsize);
        igris_f32toa(axctr->matrix[i], buf, 5);
        // snprintf(buf, bufsize, "%f ", axctr->matrix[i]);
        strncat(output, buf, outmax);
        strncat(output, " ", outmax);
    }
    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "\r\n");
    strncat(output, buf, outmax);

    return 0;
}

int axstate_linear_processor::command(int argc,
                                      char **argv,
                                      char *output,
                                      int outmax)
{
    int status = ENOENT;

    if (strcmp("matrix", argv[0]) == 0)
        status = ::matrix(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("info", argv[0]) == 0)
        status = ::info(this, argc - 1, argv + 1, output, outmax);

    if (status != ENOENT)
        return status;

    return signal_processor::command(argc, argv, output, outmax);
}

void axstate_linear_processor::deinit()
{
    for (int i = 0; i < dim(); ++i)
    {
        leftax(i)->detach_possible_controller(this);
        rightax(i)->detach_listener(this);
    }
}

void axstate_linear_processor::evaluate_invertion()
{
    matops_square_inverse_d(matrix,       // in
                            dim(),        // size, n==m
                            dim(),        // stride
                            invert_matrix // out
    );
}

heimer::axstate_linear_processor::axstate_linear_processor(
    const char *name,
    int _dim,
    axis_state **leftside,
    axis_state **rightside,
    double *matrix,
    double *invert_matrix)
    : axstate_signal_processor(name, _dim, _dim)
{
    attach_leftside_table(leftside);
    attach_rightside_table(rightside);

    this->matrix = matrix;
    this->invert_matrix = invert_matrix;
}

heimer::axstate_linear_processor::axstate_linear_processor(const char *name,
                                                           int dim)
    : axstate_linear_processor(name, dim, nullptr, nullptr, nullptr, nullptr)
{
}

void heimer::axstate_linear_processor::allocate_resources()
{
    this->attach_leftside_table(new axis_state *[dim()]);
    this->attach_rightside_table(new axis_state *[dim()]);
    this->matrix = new double[dim() * dim()];
    this->invert_matrix = new double[dim() * dim()];

    ralgo::matrix_view_ro<double> A(this->matrix, dim(), dim());
    ralgo::matrix_view_ro<double> B(this->invert_matrix, dim(), dim());

    ralgo::matops::eye(A);
    ralgo::matops::eye(B);
}

void heimer::axstate_linear_processor::on_activate(disctime_t)
{
    for (int i = 0; i < dim(); ++i)
    {
        rightax(i)->ctrpos = rightax(i)->feedpos;
        rightax(i)->ctrvel = rightax(i)->feedvel;
    }
}

int heimer::axstate_linear_processor::help(char *output, int outmax)
{
    snprintf(output,
             outmax,
             "bindleft\r\n"
             "bindright\r\n"
             "matrix\r\n"
             "info\r\n");
    return 0;
}
