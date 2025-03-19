
#ifndef OH_SURFACE_CONTROL_H
#define OH_SURFACE_CONTROL_H
#include <sys/cdefs.h>
__BEGIN_DECLS

// Add surface control type in native_node.h
enum {
  ARKUI_NODE_SURFACE_CONTROL,
};

struct OH_SurfaceControl;
/**
 * The SurfaceControl API can be used to provide a hierarchy of surfaces for
 * composition to the system compositor. OH_SurfaceControl represents a content
 * node in this hierarchy.
 */
typedef struct OH_SurfaceControl OH_SurfaceControl;

// Acquire an OH_SurfaceControl from an ArkUI_NodeHandle which type must be
// ARKUI_NODE_SURFACE_CONTROL.
OH_SurfaceControl* OH_SurfaceControl_acquireFromNode(ArkUI_NodeHandle node,
                                                     const char* debug_name);

// Create an OH_SurfaceControl.
OH_SurfaceControl* OH_SurfaceControl_create(OH_SurfaceControl* parent,
                                            const char* debug_name);
void OH_SurfaceControl_acquire(OH_SurfaceControl* surface_control);
void OH_SurfaceControl_release(OH_SurfaceControl* surface_control);

struct OH_SurfaceTransaction;
/**
 * OH_SurfaceTransaction is a collection of updates to the surface tree that
 * must be applied atomically.
 */
typedef struct OH_SurfaceTransaction OH_SurfaceTransaction;
/**
 * The caller takes ownership of the transaction and must release it using
 * OH_SurfaceTransaction_delete() below.
 */
OH_SurfaceTransaction* OH_SurfaceTransaction_create();
/**
 * Destroys the \a transaction object.
 */
void OH_SurfaceTransaction_delete(OH_SurfaceTransaction* transaction);
/**
 * Applies the updates accumulated in \a transaction.
 *
 * Note that the transaction is guaranteed to be applied atomically. The
 * transactions which are applied on the same thread are also guaranteed to be
 * applied in order.
 */
void OH_SurfaceTransaction_apply(OH_SurfaceTransaction* transaction);
/**
 * An opaque handle returned during a callback that can be used to query general
 * stats and stats for surfaces which were either removed or for which buffers
 * were updated after this transaction was applied.
 */
typedef struct OH_SurfaceTransactionStats OH_SurfaceTransactionStats;
/**
 * Since the transactions are applied asynchronously, the
 * OH_SurfaceTransaction_OnComplete callback can be used to be notified when a
 * frame including the updates in a transaction was presented.
 *
 * Buffers which are replaced or removed from the scene in the transaction
 * invoking this callback may be reused after this point.
 *
 * Starting with API level 36, prefer using \a
 * OH_SurfaceTransaction_OnBufferRelease to listen to when a buffer is ready to
 * be reused.
 *
 * \param context Optional context provided by the client that is passed into
 * the callback.
 *
 * \param stats Opaque handle that can be passed to OH_SurfaceTransactionStats
 * functions to query information about the transaction. The handle is only
 * valid during the callback.
 *
 * THREADING
 * The transaction completed callback can be invoked on any thread.
 */
typedef void (*OH_SurfaceTransaction_OnComplete)(
    void* context,
    OH_SurfaceTransactionStats* stats);
/**
 * The OH_SurfaceTransaction_OnCommit callback is invoked when transaction is
 * applied and the updates are ready to be presented. This callback will be
 * invoked before the OH_SurfaceTransaction_OnComplete callback.
 *
 * This callback does not mean buffers have been released! It simply means that
 * any new transactions applied will not overwrite the transaction for which we
 * are receiving a callback and instead will be included in the next frame. If
 * you are trying to avoid dropping frames (overwriting transactions), and
 * unable to use timestamps (Which provide a more efficient solution), then this
 * method provides a method to pace your transaction application.
 *
 * \param context Optional context provided by the client that is passed into
 * the callback.
 *
 * \param stats Opaque handle that can be passed to OH_SurfaceTransactionStats
 * functions to query information about the transaction. The handle is only
 * valid during the callback. Present and release fences are not available for
 * this callback. Querying them using
 * OH_SurfaceTransactionStats_getPresentFenceFd and
 * OH_SurfaceTransactionStats_getPreviousReleaseFenceFd will result in failure.
 *
 * THREADING
 * The transaction committed callback can be invoked on any thread.
 */
typedef void (*OH_SurfaceTransaction_OnCommit)(
    void* context,
    OH_SurfaceTransactionStats* stats);
/**
 * The OH_SurfaceTransaction_OnBufferRelease callback is invoked when a buffer
 * that was passed in OH_SurfaceTransaction_setBuffer is ready to be reused.
 *
 * This callback is guaranteed to be invoked if OH_SurfaceTransaction_setBuffer
 * is called with a non null buffer. If the buffer in the transaction is
 * replaced via another call to OH_SurfaceTransaction_setBuffer, the callback
 * will be invoked immediately. Otherwise the callback will be invoked before
 * the OH_SurfaceTransaction_OnComplete callback after the buffer was presented.
 *
 * If this callback is set, caller should not release the buffer using the
 * OH_SurfaceTransaction_OnComplete.
 *
 * \param context Optional context provided by the client that is passed into
 * the callback.
 *
 * \param release_fence_fd Returns the fence file descriptor used to signal the
 * release of buffer associated with this callback. If this fence is valid
 * (>=0), the buffer has not yet been released and the fence will signal when
 * the buffer has been released. If the fence is -1 , the buffer is already
 * released. The recipient of the callback takes ownership of the fence fd and
 * is responsible for closing it.
 *
 * THREADING
 * The callback can be invoked on any thread.
 */
typedef void (*OH_SurfaceTransaction_OnBufferRelease)(void* context,
                                                      int release_fence_fd);
/**
 * Returns the timestamp of when the frame was latched by the framework. Once a
 * frame is latched by the framework, it is presented at the following hardware
 * vsync.
 */
int64_t OH_SurfaceTransactionStats_getLatchTime(
    OH_SurfaceTransactionStats* surface_transaction_stats);
/**
 * Returns a sync fence that signals when the transaction has been presented.
 * The recipient of the callback takes ownership of the fence and is responsible
 * for closing it. If a device does not support present fences, a -1 will be
 * returned.
 *
 * This query is not valid for OH_SurfaceTransaction_OnCommit callback.
 *
 * Available since API level 29.
 */
int OH_SurfaceTransactionStats_getPresentFenceFd(
    OH_SurfaceTransactionStats* surface_transaction_stats);

void OH_SurfaceTransactionStats_getSurfaceControls(
    OH_SurfaceTransactionStats* surface_transaction_stats,
    OH_SurfaceControl*** out_surface_controls,
    size_t* out_surface_controls_size);
void OH_SurfaceTransactionStats_releaseSurfaceControls(
    OH_SurfaceControl** surface_controls);

/**
 * Sets the callback that will be invoked when the updates from this transaction
 * are presented. For details on the callback semantics and data, see the
 * comments on the OH_SurfaceTransaction_OnComplete declaration above.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setOnComplete(OH_SurfaceTransaction* transaction,
                                         void* context,
                                         OH_SurfaceTransaction_OnComplete func);
/**
 * Sets the callback that will be invoked when the updates from this transaction
 * are applied and are ready to be presented. This callback will be invoked
 * before the OH_SurfaceTransaction_OnComplete callback.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setOnCommit(OH_SurfaceTransaction* transaction,
                                       void* context,
                                       OH_SurfaceTransaction_OnCommit func);
/**
 * Reparents the \a surface_control from its old parent to the \a new_parent
 * surface control. Any children of the reparented \a surface_control will
 * remain children of the \a surface_control.
 *
 * The \a new_parent can be null. Surface controls with a null parent do not
 * appear on the display.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_reparent(OH_SurfaceTransaction* transaction,
                                    OH_SurfaceControl* surface_control,
                                    OH_SurfaceControl* new_parent);
/**
 * Parameter for OH_SurfaceTransaction_setVisibility().
 */
enum OH_SurfaceTransactionVisibility : int8_t {
  ASURFACE_TRANSACTION_VISIBILITY_HIDE = 0,
  ASURFACE_TRANSACTION_VISIBILITY_SHOW = 1,
};
/**
 * Updates the visibility of \a surface_control. If show is set to
 * ASURFACE_TRANSACTION_VISIBILITY_HIDE, the \a surface_control and all surfaces
 * in its subtree will be hidden.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setVisibility(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    enum OH_SurfaceTransactionVisibility visibility);
/**
 * Updates the z order index for \a surface_control. Note that the z order for a
 * surface is relative to other surfaces which are siblings of this surface. The
 * behavior of sibilings with the same z order is undefined.
 *
 * Z orders may be from MIN_INT32 to MAX_INT32. A layer's default z order index
 * is 0.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setZOrder(OH_SurfaceTransaction* transaction,
                                     OH_SurfaceControl* surface_control,
                                     int32_t z_order);
/**
 * Updates the AHardwareBuffer displayed for \a surface_control. If not -1, the
 * acquire_fence_fd should be a file descriptor that is signaled when all
 * pending work for the buffer is complete and the buffer can be safely read.
 *
 * The frameworks takes ownership of the \a acquire_fence_fd passed and is
 * responsible for closing it.
 *
 * Note that the buffer must be allocated with
 * AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE as the surface control might be
 * composited using the GPU.
 *
 * When the buffer is ready to be reused, the
 * OH_SurfaceTransaction_OnBufferRelease callback will be invoked. If the buffer
 * is null, the callback will not be invoked.
 *
 * Available since API level 36.
 */
void OH_SurfaceTransaction_setBufferWithRelease(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    AHardwareBuffer* buffer,
    int acquire_fence_fd,
    void* context,
    OH_SurfaceTransaction_OnBufferRelease func);
/**
 * Updates the color for \a surface_control.  This will make the background
 * color for the OH_SurfaceControl visible in transparent regions of the
 * surface.  Colors \a r, \a g, and \a b must be within the range that is valid
 * for \a dataspace.  \a dataspace and \a alpha will be the dataspace and alpha
 * set for the background color layer.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setColor(OH_SurfaceTransaction* transaction,
                                    OH_SurfaceControl* surface_control,
                                    float r,
                                    float g,
                                    float b,
                                    float alpha,
                                    enum OH_DataSpace dataspace);

/**
 * Bounds the surface and its children to the bounds specified. The crop and
 * buffer size will be used to determine the bounds of the surface. If no crop
 * is specified and the surface has no buffer, the surface bounds is only
 * constrained by the size of its parent bounds.
 *
 * \param crop The bounds of the crop to apply.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setCrop(OH_SurfaceTransaction* transaction,
                                   OH_SurfaceControl* surface_control,
                                   const ARect* crop);
/**
 * Specifies the position in the parent's space where the surface will be drawn.
 *
 * \param x The x position to render the surface.
 * \param y The y position to render the surface.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setPosition(OH_SurfaceTransaction* transaction,
                                       OH_SurfaceControl* surface_control,
                                       int32_t x,
                                       int32_t y);
/**
 * \param transform The transform applied after the source rect is applied to
 * the buffer. This parameter should be set to 0 for no transform. To specify a
 * transform use the NATIVE_WINDOW_TRANSFORM_* enum.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setBufferTransform(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    int32_t transform);
/**
 * Sets an x and y scale of a surface with (0, 0) as the centerpoint of the
 * scale.
 *
 * \param xScale The scale in the x direction. Must be greater than 0.
 * \param yScale The scale in the y direction. Must be greater than 0.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setScale(OH_SurfaceTransaction* transaction,
                                    OH_SurfaceControl* surface_control,
                                    float xScale,
                                    float yScale);
/**
 * Parameter for OH_SurfaceTransaction_setBufferTransparency().
 */
enum OH_SurfaceTransactionTransparency : int8_t {
  ASURFACE_TRANSACTION_TRANSPARENCY_TRANSPARENT = 0,
  ASURFACE_TRANSACTION_TRANSPARENCY_TRANSLUCENT = 1,
  ASURFACE_TRANSACTION_TRANSPARENCY_OPAQUE = 2,
};
/**
 * Updates whether the content for the buffer associated with this surface is
 * completely opaque. If true, every pixel of content inside the buffer must be
 * opaque or visual errors can occur.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setBufferTransparency(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    enum OH_SurfaceTransactionTransparency transparency);
/**
 * Updates the region for the content on this surface updated in this
 * transaction. If unspecified, the complete surface is assumed to be damaged.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setDamageRegion(OH_SurfaceTransaction* transaction,
                                           OH_SurfaceControl* surface_control,
                                           const ARect* rects,
                                           uint32_t count);
/**
 * Specifies a desiredPresentTime for the transaction. The framework will try to
 * present the transaction at or after the time specified.
 *
 * Transactions will not be presented until all of their acquire fences have
 * signaled even if the app requests an earlier present time.
 *
 * If an earlier transaction has a desired present time of x, and a later
 * transaction has a desired present time that is before x, the later
 * transaction will not preempt the earlier transaction.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setDesiredPresentTime(
    OH_SurfaceTransaction* transaction,
    int64_t desiredPresentTime);
/**
 * Sets the alpha for the buffer. It uses a premultiplied blending.
 *
 * The \a alpha must be between 0.0 and 1.0.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setBufferAlpha(OH_SurfaceTransaction* transaction,
                                          OH_SurfaceControl* surface_control,
                                          float alpha);
/**
 * Sets the data space of the surface_control's buffers.
 *
 * If no data space is set, the surface control defaults to ADATASPACE_SRGB.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setBufferDataSpace(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    enum OH_DataSpace data_space);
/**
 * SMPTE ST 2086 "Mastering Display Color Volume" static metadata
 *
 * When \a metadata is set to null, the framework does not use any smpte2086
 * metadata when rendering the surface's buffer.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setHdrMetadata_smpte2086(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    struct AHdrMetadata_smpte2086* metadata);
/**
 * Sets the CTA 861.3 "HDR Static Metadata Extension" static metadata on a
 * surface.
 *
 * When \a metadata is set to null, the framework does not use any cta861.3
 * metadata when rendering the surface's buffer.
 *
 * Available since API level 29.
 */
void OH_SurfaceTransaction_setHdrMetadata_cta861_3(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    struct AHdrMetadata_cta861_3* metadata);
/**
 * Sets the desired extended range brightness for the layer. This only applies
 *for layers whose dataspace has RANGE_EXTENDED set on it. See:
 *OH_SurfaceTransaction_setDesiredHdrHeadroom, prefer using this API for formats
 *that encode an HDR/SDR ratio as part of generating the buffer.
 *
 * @param surface_control The layer whose extended range brightness is being
 *specified
 * @param currentBufferRatio The current HDR/SDR ratio of the current buffer as
 *represented as peakHdrBrightnessInNits / targetSdrWhitePointInNits. For
 *example if the buffer was rendered with a target SDR whitepoint of 100nits and
 *a max display brightness of 200nits, this should be set to 2.0f.
 *
 *                           Default value is 1.0f.
 *
 *                           Transfer functions that encode their own brightness
 *ranges, such as HLG or PQ, should also set this to 1.0f and instead
 *communicate extended content brightness information via metadata such as
 *CTA861_3 or SMPTE2086.
 *
 *                           Must be finite && >= 1.0f
 *
 * @param desiredRatio The desired HDR/SDR ratio as represented as
 *peakHdrBrightnessInNits / targetSdrWhitePointInNits. This can be used to
 *communicate the max desired brightness range. This is similar to the "max
 *luminance" value in other HDR metadata formats, but represented as a ratio of
 *the target SDR whitepoint to the max display brightness. The system may not be
 *able to, or may choose not to, deliver the requested range.
 *
 *                     While requesting a large desired ratio will result in the
 *most dynamic range, voluntarily reducing the requested range can help improve
 *battery life as well as can improve quality by ensuring greater bit depth is
 *allocated to the luminance range in use.
 *
 *                     Default value is 1.0f and indicates that extended range
 *brightness is not being used, so the resulting SDR or HDR behavior will be
 *                     determined entirely by the dataspace being used (ie,
 *typically SDR however PQ or HLG transfer functions will still result in HDR)
 *
 *                     When called after
 *OH_SurfaceTransaction_setDesiredHdrHeadroom, the desiredRatio will override
 *the desiredHeadroom provided by OH_SurfaceTransaction_setDesiredHdrHeadroom.
 *Conversely, when called before OH_SurfaceTransaction_setDesiredHdrHeadroom,
 *the desiredHeadroom provided by . OH_SurfaceTransaction_setDesiredHdrHeadroom
 *will override the desiredRatio.
 *
 *                     Must be finite && >= 1.0f
 *
 * Available since API level 34.
 */
void OH_SurfaceTransaction_setExtendedRangeBrightness(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    float currentBufferRatio,
    float desiredRatio) __INTRODUCED_IN(__ANDROID_API_U__);
/**
 * Sets the desired HDR headroom for the layer. See:
 * OH_SurfaceTransaction_setExtendedRangeBrightness, prefer using this API for
 * formats that conform to HDR standards like HLG or HDR10, that do not
 * communicate a HDR/SDR ratio as part of generating the buffer.
 *
 * @param surface_control The layer whose desired HDR headroom is being
 * specified
 *
 * @param desiredHeadroom The desired HDR/SDR ratio as represented as
 * peakHdrBrightnessInNits / targetSdrWhitePointInNits. This can be used to
 * communicate the max desired brightness range of the panel. The system may not
 * be able to, or may choose not to, deliver the requested range.
 *
 *                        While requesting a large desired ratio will result in
 * the most dynamic range, voluntarily reducing the requested range can help
 *                        improve battery life as well as can improve quality by
 * ensuring greater bit depth is allocated to the luminance range in use.
 *
 *                        Default value is 0.0f and indicates that the system
 * will choose the best headroom for this surface control's content. Typically,
 * this means that HLG/PQ encoded content will be displayed with some HDR
 * headroom greater than 1.0.
 *
 *                        When called after
 * OH_SurfaceTransaction_setExtendedRangeBrightness, the desiredHeadroom will
 * override the desiredRatio provided by
 *                        OH_SurfaceTransaction_setExtendedRangeBrightness.
 * Conversely, when called before
 * OH_SurfaceTransaction_setExtendedRangeBrightness, the desiredRatio provided
 * by OH_SurfaceTransaction_setExtendedRangeBrightness will override the
 * desiredHeadroom.
 *
 *                        Must be finite && >= 1.0f or 0.0f to indicate there is
 * no desired headroom.
 *
 * Available since API level 35.
 */
void OH_SurfaceTransaction_setDesiredHdrHeadroom(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    float desiredHeadroom) __INTRODUCED_IN(__ANDROID_API_V__);
/**
 * Sets the Lut(s) to be applied for the layer.
 *
 * The function makes a deep copy of the provided `luts`.
 * Any modifications made to the `luts` object after calling this function
 * will not affect the Lut(s) applied to the layer.
 *
 * @param surface_control The layer where Lut(s) is being applied
 * @param luts The Lut(s) to be applied
 *
 * Available since API level 36.
 */
void OH_SurfaceTransaction_setLuts(OH_SurfaceTransaction* transaction,
                                   OH_SurfaceControl* surface_control,
                                   const struct ADisplayLuts* luts);

/**
 * Sets the intended frame rate for \a surface_control.
 *
 * On devices that are capable of running the display at different refresh
 * rates, the system may choose a display refresh rate to better match this
 * surface's frame rate. Usage of this API won't directly affect the
 * application's frame production pipeline. However, because the system may
 * change the display refresh rate, calls to this function may result in changes
 * to Choreographer callback timings, and changes to the time interval at which
 * the system releases buffers back to the application.
 *
 * You can register for changes in the refresh rate using
 * \a AChoreographer_registerRefreshRateCallback.
 *
 * See OH_SurfaceTransaction_clearFrameRate().
 *
 * \param frameRate is the intended frame rate of this surface, in frames per
 * second. 0 is a special value that indicates the app will accept the system's
 * choice for the display frame rate, which is the default behavior if this
 * function isn't called. The frameRate param does <em>not</em> need to be a
 * valid refresh rate for this device's display - e.g., it's fine to pass 30fps
 * to a device that can only run the display at 60fps.
 *
 * \param compatibility The frame rate compatibility of this surface. The
 * compatibility value may influence the system's choice of display frame rate.
 * To specify a compatibility use the ANATIVEWINDOW_FRAME_RATE_COMPATIBILITY_*
 * enum. This parameter is ignored when frameRate is 0.
 *
 * \param changeFrameRateStrategy Whether display refresh rate transitions
 * caused by this surface should be seamless. A seamless transition is one that
 * doesn't have any visual interruptions, such as a black screen for a second or
 * two. See the ANATIVEWINDOW_CHANGE_FRAME_RATE_* values. This parameter is
 * ignored when frameRate is 0.
 *
 * Available since API level 31.
 */
void OH_SurfaceTransaction_setFrameRateWithChangeStrategy(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    float frameRate,
    int8_t compatibility,
    int8_t changeFrameRateStrategy);
/**
 * Clears the frame rate which is set for \a surface_control.
 *
 * This is equivalent to calling
 * OH_SurfaceTransaction_setFrameRateWithChangeStrategy(
 * transaction, 0, compatibility, changeFrameRateStrategy).
 *
 * Usage of this API won't directly affect the application's frame production
 * pipeline. However, because the system may change the display refresh rate,
 * calls to this function may result in changes to Choreographer callback
 * timings, and changes to the time interval at which the system releases
 * buffers back to the application.
 *
 * See OH_SurfaceTransaction_setFrameRateWithChangeStrategy()
 *
 * You can register for changes in the refresh rate using
 * \a AChoreographer_registerRefreshRateCallback.
 *
 * See OH_SurfaceTransaction_setFrameRateWithChangeStrategy().
 *
 * Available since API level 34.
 */
void OH_SurfaceTransaction_clearFrameRate(OH_SurfaceTransaction* transaction,
                                          OH_SurfaceControl* surface_control);
/**
 * Indicate whether to enable backpressure for buffer submission to a given
 * SurfaceControl.
 *
 * By default backpressure is disabled, which means submitting a buffer prior to
 * receiving a callback for the previous buffer could lead to that buffer being
 * "dropped". In cases where you are selecting for latency, this may be a
 * desirable behavior! We had a new buffer ready, why shouldn't we show it?
 *
 * When back pressure is enabled, each buffer will be required to be presented
 * before it is released and the callback delivered
 * (absent the whole SurfaceControl being removed).
 *
 * Most apps are likely to have some sort of backpressure internally, e.g. you
 * are waiting on the callback from frame N-2 before starting frame N. In high
 * refresh rate scenarios there may not be much time between SurfaceFlinger
 * completing frame N-1 (and therefore releasing buffer N-2) and beginning frame
 * N. This means your app may not have enough time to respond in the callback.
 * Using this flag and pushing buffers earlier for server side queuing will be
 * advantageous in such cases.
 *
 * Available since API level 31.
 *
 * \param transaction The transaction in which to make the change.
 * \param surface_control The OH_SurfaceControl on which to control buffer
 * backpressure behavior.
 * \param enableBackPressure Whether to enable back pressure.
 */
void OH_SurfaceTransaction_setEnableBackPressure(
    OH_SurfaceTransaction* transaction,
    OH_SurfaceControl* surface_control,
    bool enableBackPressure);
/**
 * Sets the frame timeline to use in SurfaceFlinger.
 *
 * A frame timeline should be chosen based on the frame deadline the application
 * can meet when rendering the frame and the application's desired presentation
 * time. By setting a frame timeline, SurfaceFlinger tries to present the frame
 * at the corresponding expected presentation time.
 *
 * To receive frame timelines, a callback must be posted to Choreographer using
 * AChoreographer_postVsyncCallback(). The \c vsyncId can then be extracted from
 * the callback payload using
 * AChoreographerFrameCallbackData_getFrameTimelineVsyncId().
 *
 * Available since API level 33.
 *
 * \param vsyncId The vsync ID received from AChoreographer, setting the frame's
 * presentation target to the corresponding expected presentation time and
 * deadline from the frame to be rendered. A stale or invalid value will be
 * ignored.
 */
void OH_SurfaceTransaction_setFrameTimeline(OH_SurfaceTransaction* transaction,
                                            AVsyncId vsyncId);
__END_DECLS
#endif  // OH_SURFACE_CONTROL_H
