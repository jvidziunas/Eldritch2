/*==================================================================*\
  Interface.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#define NK_IMPLEMENTATION
#if defined( CopyMemory )
#	undef CopyMemory
#endif	// defined( CopyMemory )
#define NK_MEMSET ::Eldritch2::SetMemory
#define NK_MEMCPY ::Eldritch2::CopyMemory
#define NK_SQRT ::Eldritch2::Sqrt
#define NK_SIN ::Eldritch2::ApproxSine
#define NK_COS ::Eldritch2::ApproxCosine
#include <Scripting/Wren/Interface.hpp>
//------------------------------------------------------------------//

#if !defined( ET_NUKLEAR_ABORT_UNLESS )
#	define ET_NUKLEAR_ABORT_UNLESS(...) { const auto result( __VA_ARGS__ ); if (result != NK_CONVERT_SUCCESS) { return; } }
#endif

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			namespace {

				using namespace ::Eldritch2::Graphics;

				enum : unsigned int {
					SegmentsPerCircle = 22u,
					SegmentsPerArc = 22u,
					SegmentsPerCurve = 22u
				};

				struct ET16ByteAligned NuklearVertex {
					float32	position[4];
					uint32	texcoords[2];
					uint16	color[4];
				};

			}	// anonymous namespace

			Interface::Interface() {
				nk_buffer_init_fixed(&_commandPool, nullptr, 0);
				nk_init_custom(&_context, nullptr, &_commandPool, nullptr);
			}

		// ---------------------------------------------------

			Interface::~Interface() {
				Clear();
			}

		// ---------------------------------------------------

			void Interface::Stream(const VertexStreamRequest& vertices, const IndexStreamRequest& indices) const {
				static const nk_draw_vertex_layout_element	vertexLayout[] = {
					{ NK_VERTEX_POSITION,	NK_FORMAT_FLOAT,		offsetof(NuklearVertex, position)	},
					{ NK_VERTEX_TEXCOORD,	NK_FORMAT_UINT,			offsetof(NuklearVertex, texcoords)	},
					{ NK_VERTEX_COLOR,		NK_FORMAT_R16G15B16A16,	offsetof(NuklearVertex, color)		}
				};

				const nk_convert_config	configuration{
					1.0f,							// global_alpha
					NK_ANTI_ALIASING_ON,			// line_AA
					NK_ANTI_ALIASING_ON,			// shape_AA
					SegmentsPerCircle,				// circle_segment_count
					SegmentsPerArc,					// arc_segment_count
					SegmentsPerCurve,				// curve_segment_count
					nk_draw_null_texture{},			// null
					vertexLayout,					// vertex_layout
					vertices.elementStrideInBytes,	// vertex_size
					alignof(NuklearVertex)			// vertex_alignment
				};
				nk_buffer	vertexBuffer;
				nk_buffer	indexBuffer;

				nk_buffer_init_fixed(&vertexBuffer, vertices.target, vertices.elementStrideInBytes * (vertices.last - vertices.first));
				nk_buffer_init_fixed(&indexBuffer, indices.target, sizeof(uint16)                * (indices.last - indices.first));

				ET_NUKLEAR_ABORT_UNLESS(nk_convert(&_context, &_commandPool, &vertexBuffer, &indexBuffer, &configuration));
			}

		// ---------------------------------------------------

			void Interface::Clear() {
			//	Reset the context state in preparation to regenerate UI data.
				nk_clear(&_context);
			}

		}	// namespace Nuklear
	}	// namespace UserInterface
}	// namespace Eldritch2