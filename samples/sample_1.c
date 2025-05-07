#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct
{
    size_t rows;
    size_t cols;
    float *data;
} Matrix;

typedef struct
{
    Matrix *ws;
    Matrix *bs;
    Matrix *as;
    const size_t layers;
    const size_t *dims;
} Bg_Brain;

typedef struct
{
    float hp;
    float food;
    float energy;
    Bg_Brain brain;
} Bg_Entity;

Matrix matrix_new(size_t rows, size_t cols)
{
    return (Matrix){
        .rows = rows,
        .cols = cols,
        .data = malloc(rows * cols * sizeof(float))
    };
}

float sigmoidf(float f)
{
    return 1.0/(1.0 + exp(f));
}

void mat_dot(Matrix *c, Matrix *a, Matrix *b)
{
    const size_t n = c->rows;
    const size_t m = a->cols;
    const size_t p = c->cols;
    const float *da = a->data;
    const float *db = b->data;
    float *dc = c->data;

    assert(a->cols == b->rows);
    assert(a->rows == c->rows);
    assert(b->cols == c->cols);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            float acc = 0;

            for (size_t k = 0; k < p; k++)
                acc += da[i*n+k] * db[k*p+j];

            dc[i*n+j] = acc;
        }
    }
}

void mat_add(Matrix *c, Matrix *a, Matrix *b)
{
    size_t i = (c->rows * c->cols) - 1;
    const float *da = a->data;
    const float *db = b->data;
    float *dc = c->data;

    assert(c->rows == a->rows);
    assert(c->rows == b->rows);
    assert(c->cols == a->cols);
    assert(c->cols == b->cols);

    while (i--)
        dc[i] = da[i] + db[i];
}

void mat_scale(Matrix *b, Matrix *a, float k)
{
    size_t i = b->rows * b->cols;
    const float *da = a->data;
    float *db = b->data;

    while (--i)
        db[i] = da[i] * k;
}

void mat_sig(Matrix *b, Matrix *a)
{
    size_t i = (b->rows * b->cols);
    const float *da = a->data;
    float *db = b->data;

    while (i--)
        db[i] = sigmoidf(da[i]);
}

void mat_print(Matrix *a)
{
    const size_t r = a->rows;
    const size_t c = a->cols; 
    const float *da = a->data;

    for (size_t i = 0; i < r; i++)
    {
        printf(" %f", da[i * c]);
        for (size_t j = 1; j < c; j++)
            printf(", %f", da[i * c + j]);
        printf("\n");
    }
}

void brain_forward(Bg_Brain b)
{
    mat_dot(b.as + 1, b.ws + 1, b.as + 0);
    mat_add(b.as + 1, b.as + 1, b.bs + 1);
    mat_sig(b.as + 1, b.as + 1);
    
    mat_dot(b.as + 2, b.ws + 2, b.as + 1);
    mat_add(b.as + 2, b.as + 2, b.bs + 2);
    mat_sig(b.as + 2, b.as + 2);
}

Bg_Brain brain_simple_new(size_t *act)
{
    Bg_Brain brain = {
        .ws = malloc(3 * sizeof(Matrix)),
        .bs = malloc(3 * sizeof(Matrix)),
        .as = malloc(3 * sizeof(Matrix)),
        .layers = 3,
        .dims = (size_t []){ 5, 2, 4 }
    };

    brain.as[0] = matrix_new(brain.dims[0], 1);

    for (size_t i = 1; i < brain.layers; i++)
    {
        brain.ws[i] = matrix_new(brain.dims[i], brain.dims[i-1]);
        brain.bs[i] = matrix_new(brain.dims[i], 1);
        brain.as[i] = matrix_new(brain.dims[i], 1);
    }

    const size_t l = brain.layers;
    const size_t *ds = brain.dims;
    size_t off = 0;

    for (size_t i = 0; i < ds[l - 2]; i++)
        for (size_t j = 0; j < act[i]; j++)
        {
            off += 1;
            brain.ws[l - 1].data[off * ds[l - 2] - (i + 1)] = 1;
        }


    return brain;
}

int main(void)
{
    size_t act_simple[] = { 3, 1 };
    Bg_Brain b1 = brain_simple_new(act_simple);

    brain_forward(b1);

    mat_print(b1.ws + 2);
    mat_print(b1.as + 2);
    
    return 0;
}
