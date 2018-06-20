/*==================================================================*\
  AnimationTreeAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
namespace AssetViews {

class AnimationTreeAsset : public Assets::Asset {
    // - TYPE PUBLISHING ---------------------------------

public:
    struct Clip {};
    struct Blend {};

    // - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
    //!	Constructs this @ref AnimationTreeAsset instance.
    AnimationTreeAsset(const Utf8Char* const path);
    //!	Disable copy construction.
    AnimationTreeAsset(const AnimationTreeAsset&) = delete;

    ~AnimationTreeAsset() override = default;

    // ---------------------------------------------------

public:
    ErrorCode BindResources(const Builder& builder) override;

    void FreeResources() override;

    // - DATA MEMBERS ------------------------------------

public:
    ArrayList<Clip>  _clips;
    ArrayList<Blend> _blends;
};

} // namespace AssetViews
} // namespace Animation
} // namespace Eldritch2
