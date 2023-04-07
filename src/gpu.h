#ifndef INCLUDE_GPU_H
#define INCLUDE_GPU_H

#include "foundation.h"

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

GpuBuffer gpuBufferCreate(GpuBufferDescription *description);
void gpuBufferDestroy(GpuBuffer *arena);

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

GpuSampler gpuSamplerCreate(GpuSamplerDescription *description);
void gpuSamplerDestroy(GpuSampler *sampler);

typedef struct {
    u8 *vertex;
    u8 *pixel;
    u8 *compute;
} GpuShaderDescription;

typedef struct {
    void *impl;
} GpuShader;

GpuShader gpuShaderCreate(GpuShaderDescription *description);
void gpuShaderDestroy(GpuShader *shader);

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

Image imageCreate(ImageDescription *description);
void imageDestroy(Image *Image);

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
    GPU_IMAGE_USAGE_COPY,
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

GpuImage gpuImageCreate(GpuImageDescription *description);
GpuImage gpuImageCreateCopy(GpuImageDescription *description);
GpuImage gpuImageCreateCubeMap(GpuImageDescription *description);
GpuImage gpuImageCreateFromCPU(GpuImageDescription *description);
void gpuImageDestroy(GpuImage *GpuImage);

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

GpuPipeline gpuPipelineGraphicsCreate(GpuPipelineDescription *description);
GpuPipeline gpuPipelineComputeCreate(GpuPipelineDescription *description);
void gpuPipelineDestroy(GpuPipeline *pipeline);
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

GpuPipelineProfiler gpuPipelineProfilerCreate(GpuPipelineProfilerDescription *description);
void gpuPipelineProfilerDestroy(GpuPipelineProfiler *profiler);

typedef enum {
    GPU_COMMAND_BUFFER_TYPE_GRAPHICS,
    GPU_COMMAND_BUFFER_TYPE_COMPUTE,
    GPU_COMMAND_BUFFER_TYPE_UPLOAD,
} GpuCommandBufferType;

typedef struct {
    GpuCommandBufferType type;
    void                 *impl;
} GpuCommandBuffer;

GpuCommandBuffer GpuCommandBufferCreate(GpuCommandBufferType type);
void GpuCommandBufferDestroy(GpuCommandBuffer *buffer);

void GpuCommandBufferBindPipeline(GpuCommandBuffer *command_buffer, GpuPipeline *Pipeline);

void GpuCommandBufferBindBuffer(      GpuCommandBuffer *command_buffer, GpuBuffer *buffer);
void GpuCommandBufferBindRenderTarget(GpuCommandBuffer *command_buffer, GpuImage  *image, GpuImage *depth);

void GpuCommandBufferBindShaderResource(GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuImage   *image,   isize offset);
void GpuCommandBufferBindStorageImage(  GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuImage   *image,   isize offset);
void GpuCommandBufferBindConstantBuffer(GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuBuffer  *buffer,  isize offset);
void GpuCommandBufferBindStorageBuffer( GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuBuffer  *buffer,  isize offset);
void GpuCommandBufferBindSampler(       GpuCommandBuffer *command_buffer, GpuPipelineType type, GpuSampler *sampler, isize offset);

void GpuCommandBufferClearColor(GpuCommandBuffer *command_buffer, GpuImage *image, f32 red,   f32 green, f32 blue, f32 alpha);
void GpuCommandBufferClearDepth(GpuCommandBuffer *command_buffer, GpuImage *image, f32 depth, f32 stencil);

void GpuCommandBufferSetViewport(GpuCommandBuffer *command_buffer, f32 width, f32 height, f32 x, f32 y);

void GpuCommandBufferDraw(       GpuCommandBuffer *command_buffer, isize vertex_count);
void GpuCommandBufferDrawIndexed(GpuCommandBuffer *command_buffer, isize index_count);

void GpuCommandBufferDispatch(GpuCommandBuffer *command_buffer, isize x, isize y, isize z);

void GpuCommandBufferBeginPipelineStatistics(GpuCommandBuffer *command_buffer, GpuPipelineProfiler *profiler);
void GpuCommandBufferEndPipelineStatistics(  GpuCommandBuffer *command_buffer, GpuPipelineProfiler *profiler);

void GpuCommandBufferBufferBarrier(GpuCommandBuffer *command_buffer, GpuBuffer *buffer, GpuBufferLayout old_layout, GpuBufferLayout new_layout);
void GpuCommandBufferImageBarrier( GpuCommandBuffer *command_buffer, GpuImage  *image,  GpuImageLayout  new_layout);

void GpuCommandBufferBlit(GpuCommandBuffer *command_buffer, GpuImage *source, GpuImage  *destination);

void GpuCommandBufferCopyBufferToTexture(GpuCommandBuffer *command_buffer, GpuBuffer *source, GpuImage  *destination);
void GpuCommandBufferCopyTextureToBuffer(GpuCommandBuffer *command_buffer, GpuImage  *source, GpuBuffer *destination);
void GpuCommandBufferCopyBufferToBuffer( GpuCommandBuffer *command_buffer, GpuBuffer *source, GpuBuffer *destination);

void GpuCommandBufferBegin(GpuCommandBuffer *command_buffer);
void GpuCommandBufferEnd(  GpuCommandBuffer *command_buffer);
void GpuCommandBufferFlush(GpuCommandBuffer *command_buffer);

void GpuCommandBufferScreenshot(GpuCommandBuffer *command_buffer, GpuImage *Image, GpuBuffer *Temporary);

typedef enum {
    GPU_IMPL_DIRECT3D_12,
    GPU_IMPL_VULKAN,
} GpuImpl;

GpuImpl gpuGetImpl();

void gpuInit();
void gpuExit();
void gpuBeginFrame();
void gpuEndFrame();
void gpuResize(isize width, isize height);
void gpuPresent();
void gpuWait();

typedef struct {
    union {
        struct {
            isize x;
            isize y;
        };

        // ...
    };
} Vector2i;

Vector2i gpuGetDimensions();
GpuCommandBuffer* gpuGetImageCommandBuffer();
GpuImage* gpuGetSwapChainImage();

#endif // INCLUDE_GPU_H
