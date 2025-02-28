// 创建一个Node（surface control），他的父节点是一个native window
ASurfaceControl* _Nullable ASurfaceControl_createFromWindow(ANativeWindow* _Nonnull parent,
                                                            const char* _Nonnull debug_name)



// 创建一个Node，他的父节点是一个另外一个node（surface control）
ASurfaceControl* _Nullable ASurfaceControl_create(ASurfaceControl* _Nonnull parent,
                                                  const char* _Nonnull debug_name)
        __INTRODUCED_IN(29);


// acquire or remove 引用
void ASurfaceControl_acquire(ASurfaceControl* _Nonnull surface_control) __INTRODUCED_IN(31);
void ASurfaceControl_release(ASurfaceControl* _Nonnull surface_control) __INTRODUCED_IN(29);

// 创建、删除一个 surface 事务
ASurfaceTransaction* _Nonnull ASurfaceTransaction_create() __INTRODUCED_IN(29);
void ASurfaceTransaction_delete(ASurfaceTransaction* _Nullable transaction) __INTRODUCED_IN(29);

// 应用一个事务。把事务里多包涵的所有操作，一起提交给系统合成器
void ASurfaceTransaction_apply(ASurfaceTransaction* _Nonnull transaction) __INTRODUCED_IN(29);

// 事务内容已经显示
typedef void (*ASurfaceTransaction_OnComplete)(void* _Null_unspecified context,
                                               ASurfaceTransactionStats* _Nonnull stats)
        __INTRODUCED_IN(29);

// 当事务的内容已经被应用并且已经准备好显示。
typedef void (*ASurfaceTransaction_OnCommit)(void* _Null_unspecified context,
                                             ASurfaceTransactionStats* _Nonnull stats)
        __INTRODUCED_IN(31);


// 控制一个surface是否可见
void ASurfaceTransaction_setVisibility(ASurfaceTransaction* _Nonnull transaction,
                                       ASurfaceControl* _Nonnull surface_control,
                                       enum ASurfaceTransactionVisibility visibility)
        __INTRODUCED_IN(29);


// 改变一个surface的parent surface
void ASurfaceTransaction_reparent(ASurfaceTransaction* _Nonnull transaction,
                                  ASurfaceControl* _Nonnull surface_control,
                                  ASurfaceControl* _Nullable new_parent) __INTRODUCED_IN(29);

// 改变一个surface的在其他兄弟surface里的zorder
void ASurfaceTransaction_setZOrder(ASurfaceTransaction* _Nonnull transaction,
                                   ASurfaceControl* _Nonnull surface_control, int32_t z_order)
        __INTRODUCED_IN(29);

// 设置一个surface的内容buffer，
void ASurfaceTransaction_setBufferWithRelease(
  ASurfaceTransaction *_Nonnull transaction,
  ASurfaceControl *_Nonnull surface_control,
  AHardwareBuffer *_Nonnull buffer,
  int acquire_fence_fd,
  void *_Null_unspecified context,
  ASurfaceTransaction_OnBufferRelease _Nonnull func
)

// 设置surface相对于parent的位置
void ASurfaceTransaction_setPosition(ASurfaceTransaction* _Nonnull transaction,
                                     ASurfaceControl* _Nonnull surface_control, int32_t x,
                                     int32_t y) __INTRODUCED_IN(31);

/**
 * \param transform The transform applied after the source rect is applied to the buffer. This
 * parameter should be set to 0 for no transform. To specify a transform use the
 * NATIVE_WINDOW_TRANSFORM_* enum.
 *
 * Available since API level 31.
 */
void ASurfaceTransaction_setBufferTransform(ASurfaceTransaction* _Nonnull transaction,
                                            ASurfaceControl* _Nonnull surface_control,
                                            int32_t transform) __INTRODUCED_IN(31);

/**
 * Bounds the surface and its children to the bounds specified. The crop and buffer size will be
 * used to determine the bounds of the surface. If no crop is specified and the surface has no
 * buffer, the surface bounds is only constrained by the size of its parent bounds.
 *
 * \param crop The bounds of the crop to apply.
 *
 * Available since API level 31.
 */
void ASurfaceTransaction_setCrop(ASurfaceTransaction* _Nonnull transaction,
                                 ASurfaceControl* _Nonnull surface_control, const ARect& crop)
        __INTRODUCED_IN(31);

/**
 * Sets an x and y scale of a surface with (0, 0) as the centerpoint of the scale.
 *
 * \param xScale The scale in the x direction. Must be greater than 0.
 * \param yScale The scale in the y direction. Must be greater than 0.
 *
 * Available since API level 31.
 */
void ASurfaceTransaction_setScale(ASurfaceTransaction* _Nonnull transaction,
                                  ASurfaceControl* _Nonnull surface_control, float xScale,
                                  float yScale) __INTRODUCED_IN(31);

/**
 * Updates whether the content for the buffer associated with this surface is
 * completely opaque. If true, every pixel of content inside the buffer must be
 * opaque or visual errors can occur.
 *
 * Available since API level 29.
 */
void ASurfaceTransaction_setBufferTransparency(ASurfaceTransaction* _Nonnull transaction,
                                               ASurfaceControl* _Nonnull surface_control,
                                               enum ASurfaceTransactionTransparency transparency)
                                               __INTRODUCED_IN(29);

/**
 * Updates the region for the content on this surface updated in this
 * transaction. If unspecified, the complete surface is assumed to be damaged.
 *
 * Available since API level 29.
 */
void ASurfaceTransaction_setDamageRegion(ASurfaceTransaction* _Nonnull transaction,
                                         ASurfaceControl* _Nonnull surface_control,
                                         const ARect* _Nullable rects, uint32_t count)
                                         __INTRODUCED_IN(29);

/**
 * Sets the data space of the surface_control's buffers.
 *
 * If no data space is set, the surface control defaults to ADATASPACE_SRGB.
 *
 * Available since API level 29.
 */
// 可选：显示HDR内容需要。
void ASurfaceTransaction_setBufferDataSpace(ASurfaceTransaction* _Nonnull transaction,
                                            ASurfaceControl* _Nonnull surface_control,
                                            enum ADataSpace data_space) __INTRODUCED_IN(29);

/**
 * SMPTE ST 2086 "Mastering Display Color Volume" static metadata
 *
 * When \a metadata is set to null, the framework does not use any smpte2086 metadata when rendering
 * the surface's buffer.
 *
 * Available since API level 29.
 */
void ASurfaceTransaction_setHdrMetadata_smpte2086(ASurfaceTransaction* _Nonnull transaction,
                                                  ASurfaceControl* _Nonnull surface_control,
                                                  struct AHdrMetadata_smpte2086* _Nullable metadata)
                                                  __INTRODUCED_IN(29);

/**
 * Sets the CTA 861.3 "HDR Static Metadata Extension" static metadata on a surface.
 *
 * When \a metadata is set to null, the framework does not use any cta861.3 metadata when rendering
 * the surface's buffer.
 *
 * Available since API level 29.
 */
void ASurfaceTransaction_setHdrMetadata_cta861_3(ASurfaceTransaction* _Nonnull transaction,
                                                 ASurfaceControl* _Nonnull surface_control,
                                                 struct AHdrMetadata_cta861_3* _Nullable metadata)
                                                 __INTRODUCED_IN(29);

/**
 * Sets the desired extended range brightness for the layer. This only applies for layers whose
 * dataspace has RANGE_EXTENDED set on it.
 *
 * Available since API level 34.
 *
 * @param surface_control The layer whose extended range brightness is being specified
 * @param currentBufferRatio The current hdr/sdr ratio of the current buffer as represented as
 *                           peakHdrBrightnessInNits / targetSdrWhitePointInNits. For example if the
 *                           buffer was rendered with a target SDR whitepoint of 100nits and a max
 *                           display brightness of 200nits, this should be set to 2.0f.
 *
 *                           Default value is 1.0f.
 *
 *                           Transfer functions that encode their own brightness ranges, such as
 *                           HLG or PQ, should also set this to 1.0f and instead communicate
 *                           extended content brightness information via metadata such as CTA861_3
 *                           or SMPTE2086.
 *
 *                           Must be finite && >= 1.0f
 *
 * @param desiredRatio The desired hdr/sdr ratio as represented as peakHdrBrightnessInNits /
 *                     targetSdrWhitePointInNits. This can be used to communicate the max desired
 *                     brightness range. This is similar to the "max luminance" value in other
 *                     HDR metadata formats, but represented as a ratio of the target SDR whitepoint
 *                     to the max display brightness. The system may not be able to, or may choose
 *                     not to, deliver the requested range.
 *
 *                     While requesting a large desired ratio will result in the most
 *                     dynamic range, voluntarily reducing the requested range can help
 *                     improve battery life as well as can improve quality by ensuring
 *                     greater bit depth is allocated to the luminance range in use.
 *
 *                     Default value is 1.0f and indicates that extended range brightness
 *                     is not being used, so the resulting SDR or HDR behavior will be
 *                     determined entirely by the dataspace being used (ie, typically SDR
 *                     however PQ or HLG transfer functions will still result in HDR)
 *
 *                     Must be finite && >= 1.0f
 *
 * Available since API level 34.
 */
void ASurfaceTransaction_setExtendedRangeBrightness(ASurfaceTransaction* _Nonnull transaction,
                                                    ASurfaceControl* _Nonnull surface_control,
                                                    float currentBufferRatio, float desiredRatio)
                                                    __INTRODUCED_IN(__ANDROID_API_U__);
