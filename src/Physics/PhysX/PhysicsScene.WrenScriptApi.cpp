/*==================================================================*\
  PhysicsScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace {

	using namespace ::physx;

	template <typename Hit, PxU32 batchSize = 8u>
	class WrenHitCallback : public PxHitCallback<Hit> {
	// - CONSTRUCTOR/DESTRUCTOR -------------------------

	public:
	//!	Constructs this @ref WrenHitCallback instance.
		WrenHitCallback(
			WrenVM* vm,
			WrenHandle* callback,
			WrenHandle* unaryCallHandle
		) : PxHitCallback<Hit>( _buffer, _countof( _buffer ) ),
			_vm( vm ),
			_callback( callback ),
			_unaryCallHandle( unaryCallHandle ),
			_errorObject( nullptr ) {}
	//!	Disable copy construction.
		WrenHitCallback( const WrenHitCallback& ) = delete;

		~WrenHitCallback() override {
			wrenReleaseHandle( _vm, _callback );

			if (_errorObject) {
				wrenSetSlotHandle( _vm, 0, _errorObject );
				wrenAbortFiber( _vm, 0 );
			}
		}

	// ---------------------------------------------------

	public:
		PxAgain	processTouches( const Hit* buffer, PxU32 nbHits ) override {
			while (nbHits) {
				wrenSetSlotHandle( _vm, 0, _callback );
			//	Set slot to contact info object.
				wrenSetSlotNull( _vm, 1 );
				if (wrenCall( _vm, _unaryCallHandle ) != WREN_RESULT_SUCCESS) {
					_errorObject = wrenGetSlotHandle( _vm, 0 );
					return false;
				}

				if (wrenGetSlotBool( _vm, 0 ) == false) {
					return false;
				}
			}

			return true;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		WrenVM*		_vm;
		WrenHandle*	_callback;
		WrenHandle*	_unaryCallHandle;
		WrenHandle*	_errorObject;
		Hit			_buffer[batchSize];
	};

}	// anonymous namespace

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS( PhysicsScene ) {
		api.CreateClass<PhysicsScene>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "_PhysicsScene",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( Vector, Vector, double, WrenHandle )>( "eachAlongRay", [] ( WrenVM* vm ) {
					WrenHitCallback<PxRaycastHit>	callback( vm, wrenGetSlotHandle( vm, 4 ), AsContext( vm ).GetUnaryCallStub() );

					GetSlotAs<PhysicsScene>( vm, 0 ).SweepRay(
						GetSlotAs<Vector>( vm, 1 ),
						GetSlotAs<Vector>( vm, 2 ),
						static_cast<float32>(wrenGetSlotDouble( vm, 3 )),
						callback
					);
				} ),
				DefineMethod<void ( Transformation, double, WrenHandle )>( "eachAlongRay", [] ( WrenVM* vm ) {
					WrenHitCallback<PxRaycastHit>	callback( vm, wrenGetSlotHandle( vm, 3 ), AsContext( vm ).GetUnaryCallStub() );

					GetSlotAs<PhysicsScene>( vm, 0 ).SweepRay(
						GetSlotAs<Transformation>( vm, 1 ).translation,
						GetSlotAs<Transformation>( vm, 1 ).rotation.GetForward(),
						static_cast<float32>(wrenGetSlotDouble( vm, 2 )),
						callback
					);
				} ),
				DefineMethod<void ( PhysicsShape, Transformation, Vector, double, WrenHandle )>( "eachAlongShape", [] ( WrenVM* vm ) {
					WrenHitCallback<PxSweepHit>	callback( vm, wrenGetSlotHandle( vm, 5 ), AsContext( vm ).GetUnaryCallStub() );

					GetSlotAs<PhysicsScene>( vm, 0 ).SweepGeometry( 
						GetSlotAs<PhysicsShape>( vm, 1 ).asGeometry,
						GetSlotAs<Transformation>( vm, 2 ),
						GetSlotAs<Vector>( vm, 3 ),
						static_cast<float32>(wrenGetSlotDouble( vm, 4 )),
						callback
					);
				} ),
				DefineMethod<void ( PhysicsShape, Transformation, WrenHandle )>( "eachWithinShape", [] ( WrenVM* vm ) {
					WrenHitCallback<PxOverlapHit>	callback( vm, wrenGetSlotHandle( vm, 3 ), AsContext( vm ).GetUnaryCallStub() );

					GetSlotAs<PhysicsScene>( vm, 0 ).CheckOverlap( 
						GetSlotAs<PhysicsShape>( vm, 1 ).asGeometry,
						GetSlotAs<Transformation>( vm, 2 ),
						callback
					);
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Graphics
}	// namespace Eldritch2