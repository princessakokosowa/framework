#ifndef INCLUDE_GPU_H
#define INCLUDE_GPU_H

#include "basic.h"

typedef enum {
    GPU_BUFFER_TYPE_VERTEX,
    GPU_BUFFER_TYPE_INDEX,
    GPU_BUFFER_TYPE_UNIFORM,
    GPU_BUFFER_TYPE_STORAGE,
} GpuBufferType;

typedef struct {
    GpuBufferType type;
    isize         size;
    isize         stride;
} GpuBufferDescription;

typedef struct {
    GpuBufferType type;
    isize         size;
    isize         stride;
    void          *impl;
} GpuBuffer;

GpuBuffer GpuBuffer_create(GpuBufferDescription *description);
void GpuBuffer_destroy(GpuBuffer *arena);

// @TODO
// Move that to a separate header file.
typedef struct {
    isize which_heap;

    D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
    D3D12_INDEX_BUFFER_VIEW  index_buffer_view;

    // @TODO
    // `constant_buffer_view` to `cbv` and `unordered_access_view` -> `uav`?
    D3D12_CONSTANT_BUFFER_VIEW_DESC  constant_buffer_view_description;
    D3D12_UNORDERED_ACCESS_VIEW_DESC unordered_access_view_description;

    // @TODO Port Direct3D 12 Memory Allocator to C.
    // D3D12MA_Allocation *allocation;

    ID3D12Resource *resource;
} GpuBufferImpl_Direct3D12;

typedef enum {
    GPU_TEXTURE_ADDRESS_WRAP,
    GPU_TEXTURE_ADDRESS_MIRROR,
    GPU_TEXTURE_ADDRESS_CLAMP,
    GPU_TEXTURE_ADDRESS_BORDER,
} GpuTextureAddress;

typedef enum {
    GPU_TEXTURE_FILTER_NEAREST,
    GPU_TEXTURE_FILTER_LINEAR,
    GPU_TEXTURE_FILTER_ANISOTROPIC,
} GpuTextureFilter;

typedef struct {
    GpuTextureAddress address;
    GpuTextureFilter  filter;
} GpuSamplerDescription;

typedef struct {
    GpuTextureAddress address;
    GpuTextureFilter  filter;
    void              *impl;
} GpuSampler;

GpuSampler GpuSampler_create(GpuSamplerDescription *description);
void GpuSampler_destroy(GpuSampler *sampler);

typedef struct {
    u8 *vertex;
    u8 *pixel;
    u8 *compute;
} GpuShaderDescription;

typedef struct {
    void *impl;
} GpuShader;

GpuShader GpuShader_create(GpuShaderDescription *description);
void GpuShader_destroy(GpuShader *shader);

typedef struct {
    u8 *path;
} ImageDescription;

typedef struct {
    isize width;
    isize height;
    isize channels;
    bool  is_f32;
    void  *data;
} Image;

Image Image_create(ImageDescription *description);
void Image_destroy(Image *Image);

typedef enum {
    GPU_IMAGE_FORMAT_RGBA8,
    GPU_IMAGE_FORMAT_RGBA32F32,
    GPU_IMAGE_FORMAT_RGBA16F32,
    GPU_IMAGE_FORMAT_R32DEPTH,
} GpuImageFormat;

typedef enum {
    GPU_IMAGE_LAYOUT_COMMON,
    GPU_IMAGE_LAYOUT_SHADERRESOURCE,
    GPU_IMAGE_LAYOUT_STORAGE,
    GPU_IMAGE_LAYOUT_DEPTH,
    GPU_IMAGE_LAYOUT_RENDERTARGET,
    GPU_IMAGE_LAYOUT_COPYSOURCE,
    GPU_IMAGE_LAYOUT_COPYDEST,
    GPU_IMAGE_LAYOUT_PRESENT,
    GPU_IMAGE_LAYOUT_GENERICREAD,
} GpuImageLayout;

typedef GpuImageLayout GpuBufferLayout;

typedef enum {
    GPU_IMAGE_USAGE__copy,
    GPU_IMAGE_USAGE_RENDERTARGET,
    GPU_IMAGE_USAGE_DEPTHTARGET,
    GPU_IMAGE_USAGE_STORAGE,
    GPU_IMAGE_USAGE_SHADERRESOURCE,
} GpuImageUsage;

typedef struct {
    GpuImageFormat format;
    GpuImageUsage  usage;
    isize          width;
    isize          height;
    Image          *maybe_image;
} GpuImageDescription;

typedef struct {
    isize          width;
    isize          height;
    GpuImageFormat format;
    GpuImageLayout layout;
    GpuImageUsage  usage;
    void           *impl;
} GpuImage;

GpuImage GpuImage_create(GpuImageDescription *description);
GpuImage GpuImage_create_copy(GpuImageDescription *description);
GpuImage GpuImage_createCubeMap(GpuImageDescription *description);
GpuImage GpuImage_createFromCpu(GpuImageDescription *description);
void GpuImage_destroy(GpuImage *GpuImage);

typedef enum {
    GPU_PIPELINE_TYPE_GRAPHICS,
    GPU_PIPELINE_TYPE_COMPUTE,
} GpuPipelineType;

typedef enum {
    FILL_MODE_SOLID,
    FILL_MODE_LINE,
} FillMode;

typedef enum {
    CULL_MODE_NONE,
    CULL_MODE_BACK,
    CULL_MODE_FRONT,
} CullMode;

typedef enum {
    DEPTH_FUNC_GREATER,
    DEPTH_FUNC_LESS,
    DEPTH_FUNC_LESSEQUAL,
    DEPTH_FUNC_NONE,
} DepthFunc;

typedef struct {
    bool            has_depth;
    GpuPipelineType type;
    FillMode        fill_mode;
    CullMode        cull_mode;
    DepthFunc       depth_func;
    GpuShader       *shader;
    GpuImageFormat  *image_formats;
    GpuImageFormat  depth_format;
} GpuPipelineDescription;

typedef struct {
    bool            has_depth;
    GpuPipelineType type;
    FillMode        fill_mode;
    CullMode        cull_mode;
    DepthFunc       depth_func;
    GpuShader       *shader;
    GpuImageFormat  *image_formats;
    GpuImageFormat  depth_format;
    void            *impl;
} GpuPipeline;

GpuPipeline GpuPipeline_create(GpuPipelineDescription *description);
void GpuPipeline_destroy(GpuPipeline *pipeline);
// isize gpuPipelineGetDescriptor(GpuPipeline *Pipeline, u8 *name);

typedef struct {
    isize ia_vertices;
    isize ia_primitives;
    isize vs_invocations;
    isize gs_invocations;
    isize gs_primitives;
    isize c_invocations;
    isize c_primitives;
    isize ps_invocations;
    isize hs_invocations;
    isize ds_invocations;
    isize cs_invocations;
} GpuPipelineStatistics;

typedef struct {
    GpuPipelineStatistics statistics;
} GpuPipelineProfilerDescription;

typedef struct {
    GpuPipelineStatistics statistics;
    void                  *impl;
} GpuPipelineProfiler;

GpuPipelineProfiler GpuPipelineProfiler_create(GpuPipelineProfilerDescription *description);
void GpuPipelineProfiler_destory(GpuPipelineProfiler *profiler);

typedef enum {
    GPU_COMMAND_BUFFER_TYPE_GRAPHICS,
    GPU_COMMAND_BUFFER_TYPE_COMPUTE,
    GPU_COMMAND_BUFFER_TYPE_UPLOAD,
} GpuCommandBufferType;

typedef struct {
    GpuCommandBufferType type;
    void                 *impl;
} GpuCommandBuffer;

GpuCommandBuffer GpuCommandBuffer_create(GpuCommandBufferType type);
void GpuCommandBuffer_destroy(GpuCommandBuffer *buffer);

void GpuCommandBuffer_bindPipeline(GpuCommandBuffer *command_buffer, GpuPipeline *Pipeline);

void GpuCommandBuffer_bindBuffer(      GpuCommandBuffer *command_buffer, GpuBuffer *buffer);
void GpuCommandBuffer_bindR_enderTarget(GpuCommandBuffer *command_buffer, GpuImage  *image, GpuImage *depth);

void GpuCommandBuffer_bindShaderResource(GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuImage   *image,   isize off_set);
void GpuCommandBuffer_bindStorageImage(  GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuImage   *image,   isize off_set);
void GpuCommandBuffer_bindConstantBuffer(GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuBuffer  *buffer,  isize off_set);
void GpuCommandBuffer_bindStorageBuffer( GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuBuffer  *buffer,  isize off_set);
void GpuCommandBuffer_bindSampler(       GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuSampler *sampler, isize off_set);

void GpuCommandBuffer_clearColor(GpuCommandBuffer *command_buffer, GpuImage *image, f32 red,   f32 green, f32 blue, f32 alpha);
void GpuCommandBuffer_clearDepth(GpuCommandBuffer *command_buffer, GpuImage *image, f32 depth, f32 stencil);

void GpuCommandBuffer_setViewport(GpuCommandBuffer *command_buffer, f32 width, f32 height, f32 x, f32 y);

void GpuCommandBuffer_draw(       GpuCommandBuffer *command_buffer, isize vertex_count);
void GpuCommandBuffer_drawIndexed(GpuCommandBuffer *command_buffer, isize index_count);

void GpuCommandBuffer_dispatch(GpuCommandBuffer *command_buffer, isize x, isize y, isize z);

void GpuCommandBuffer_beginPipelineStatistics(GpuCommandBuffer *command_buffer, GpuPipelineProfiler *profiler);
void GpuCommandBuffer_endPipelineStatistics(  GpuCommandBuffer *command_buffer, GpuPipelineProfiler *profiler);

void GpuCommandBuffer_createBufferBarrier(GpuCommandBuffer *command_buffer, GpuBuffer *buffer, GpuBufferLayout old_layout, GpuBufferLayout new_layout);
void GpuCommandBuffer_createImageBarrier( GpuCommandBuffer *command_buffer, GpuImage  *image,  GpuImageLayout  new_layout);

void GpuCommandBuffer_blit(GpuCommandBuffer *command_buffer, GpuImage *source, GpuImage  *destination);

void GpuCommandBuffer_copyBufferToTexture(GpuCommandBuffer *command_buffer, GpuBuffer *source, GpuImage  *destination);
void GpuCommandBuffer_copyTextureToBuffer(GpuCommandBuffer *command_buffer, GpuImage  *source, GpuBuffer *destination);
void GpuCommandBuffer_copyBufferToBuffer( GpuCommandBuffer *command_buffer, GpuBuffer *source, GpuBuffer *destination);

void GpuCommandBuffer_begin(GpuCommandBuffer *command_buffer);
void GpuCommandBuffer_end(  GpuCommandBuffer *command_buffer);
void GpuCommandBuffer_flush(GpuCommandBuffer *command_buffer);

void GpuCommandBuffer_makeScreenshot(GpuCommandBuffer *command_buffer, GpuImage *Image, GpuBuffer *Temporary);

typedef enum {
    GPU_IMPL_DIRECT3D_12,
    GPU_IMPL_VULKAN,
} GpuImpl;

GpuImpl Gpu_getImpl(void);

void Gpu_create(void);
void Gpu_destroy(void);

void Gpu_beginFrame(void);
void Gpu_endFrame(void);

void Gpu_resize(isize width, isize height);
void Gpu_present(void);
void Gpu_wait(void);

typedef struct {
    union {
        struct {
            isize x;
            isize y;
        };

        // ...
    };
} Vector2i;

Vector2i          Gpu_getDimensions(void);
GpuCommandBuffer *Gpu_getImageCommandBuffer(void);
GpuImage         *Gpu_getSwapChainImage(void);

#endif // INCLUDE_GPU_H
