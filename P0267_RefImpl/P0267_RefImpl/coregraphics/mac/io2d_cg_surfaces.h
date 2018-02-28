#ifndef _IO2D_CG_SURFACES_
#define _IO2D_CG_SURFACES_

#include "io2d.h"

namespace std::experimental::io2d { inline namespace v1 { namespace _CoreGraphics {

CGContextRef _CreateBitmap(io2d::format fmt, int width, int height) noexcept;
CGContextRef _LoadBitmap(const filesystem::path &p, image_file_format iff, io2d::format fmt, ::std::error_code& ec);
    
void _WriteBitmap(CGContextRef ctx, const filesystem::path &p, image_file_format iff, ::std::error_code &ec);
CGColorRef _ClearColor();
void _Clear(CGContextRef ctx, CGColorRef with_color, CGRect in_rect );
void _Stroke(CGContextRef ctx, const basic_brush<_GS>& b, const basic_interpreted_path<_GS>& ip, const basic_brush_props<_GS>& bp, const basic_stroke_props<_GS>& sp, const basic_dashes<_GS>& d, const basic_render_props<_GS>& rp, const basic_clip_props<_GS>& cl);
void _Paint(CGContextRef ctx, const basic_brush<_GS>& b, const basic_brush_props<_GS>& bp, const basic_render_props<_GS>& rp, const basic_clip_props<_GS>& cl);
void _Fill(CGContextRef ctx, const basic_brush<_GS>& b, const basic_interpreted_path<_GS>& ip, const basic_brush_props<_GS>& bp, const basic_render_props<_GS>& rp, const basic_clip_props<_GS>& cl);
    
inline basic_display_point<GraphicsMath> _GS::surfaces::max_dimensions() noexcept {
    return basic_display_point<GraphicsMath>(16384, 16384);
}
            
inline _GS::surfaces::image_surface_data_type _GS::surfaces::create_image_surface(io2d::format fmt, int width, int height) {
    auto context = _CreateBitmap(fmt, width, height);
    if( context == nullptr )
        throw ::std::runtime_error("Failed to create an image surface");

    CGContextConcatCTM(context, CGAffineTransform{ 1., 0., 0., -1., 0., double(height) } );
    CGContextSetAllowsAntialiasing(context, true);
    
    image_surface_data_type data;
    data.context.reset(context);
    data.dimensions.x(width);
    data.dimensions.y(height);
    data.format = fmt;
    return data;
}
    
inline _GS::surfaces::image_surface_data_type
_GS::surfaces::create_image_surface(filesystem::path p, image_file_format iff, io2d::format fmt) {
    ::std::error_code ec;
    auto data = create_image_surface(p, iff, fmt, ec);
    if( ec )
        throw ::std::system_error(ec);
    return data;
}
    
inline _GS::surfaces::image_surface_data_type
_GS::surfaces::create_image_surface(filesystem::path p, image_file_format iff, io2d::format fmt, ::std::error_code& ec) noexcept {
    auto context = _LoadBitmap(p, iff, fmt, ec);
    if( !context )
        return {};

    auto width = (int)CGBitmapContextGetWidth(context);
    auto height = (int)CGBitmapContextGetHeight(context);
    
    CGContextConcatCTM(context, CGAffineTransform{ 1., 0., 0., -1., 0., double(height) } );
    CGContextSetAllowsAntialiasing(context, true);
    
    image_surface_data_type data;
    data.context.reset(context);
    data.dimensions.x(width);
    data.dimensions.y(height);
    data.format = fmt;
    return data;
}

inline _GS::surfaces::image_surface_data_type
_GS::surfaces::move_image_surface(image_surface_data_type&& data) noexcept {
    return move(data);
}
    
inline void
_GS::surfaces::destroy(image_surface_data_type& /*data*/) noexcept {
    // Do nothing.
}
            
inline basic_display_point<GraphicsMath>
_GS::surfaces::dimensions(const image_surface_data_type& data) noexcept {
    return data.dimensions;
}
            
inline void _GS::surfaces::save(image_surface_data_type& data, const filesystem::path &p, image_file_format iff) {
    ::std::error_code ec;
    save(data, p, iff, ec);
    if( ec )
        throw ::std::system_error(ec);
}
            
inline void _GS::surfaces::save(image_surface_data_type& data, const filesystem::path &p, image_file_format iff, error_code& ec) noexcept {
    _WriteBitmap(data.context.get(), p, iff, ec);
}
            
inline io2d::format _GS::surfaces::format(const image_surface_data_type& data) noexcept {
    return data.format;
}
            
inline void _GS::surfaces::flush(image_surface_data_type& data) {
    ::std::error_code ec;
    flush(data, ec);
    if( ec )
        throw ::std::system_error(ec);
}
            
inline void _GS::surfaces::flush(image_surface_data_type& data, error_code& ec) noexcept {
    CGContextFlush(data.context.get());
}
            
inline void _GS::surfaces::clear(image_surface_data_type& data) {
    _Clear(data.context.get(), _ClearColor(), CGRectMake(0, 0, data.dimensions.x(), data.dimensions.y()) );
}

inline void _GS::surfaces::stroke(image_surface_data_type& data, const basic_brush<_GS>& b, const basic_interpreted_path<_GS>& ip, const basic_brush_props<_GS>& bp, const basic_stroke_props<_GS>& sp, const basic_dashes<_GS>& d, const basic_render_props<_GS>& rp, const basic_clip_props<_GS>& cl) {
    _Stroke(data.context.get(), b, ip, bp, sp, d, rp, cl);
}
    
inline void _GS::surfaces::fill(image_surface_data_type& data, const basic_brush<_GS>& b, const basic_interpreted_path<_GS>& ip, const basic_brush_props<_GS>& bp, const basic_render_props<_GS>& rp, const basic_clip_props<_GS>& cl) {
    _Fill(data.context.get(), b, ip, bp, rp, cl);
}
            
} // namespace _CoreGraphics
} // inline namespace v1
} // std::experimental::io2d

#endif