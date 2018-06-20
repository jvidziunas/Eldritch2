/*==================================================================*\
  KeyframeClipAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
namespace AssetViews {

class KeyframeClipAsset : public Assets::Asset {
    // - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
    //!	Constructs this @ref KeyframeClipAsset instance.
    KeyframeClipAsset(const Utf8Char* const filePath);
    //!	Disable copy construction.
    KeyframeClipAsset(const KeyframeClipAsset&) = delete;

    ~KeyframeClipAsset() override = default;

    // ---------------------------------------------------

public:
    ErrorCode BindResources(const Builder& builder) override;

    void FreeResources() override;

    // ---------------------------------------------------

    //!	Disable copy assignment.
    KeyframeClipAsset& operator=(const KeyframeClipAsset&) = delete;
};

} // namespace AssetViews
} // namespace Animation
} // namespace Eldritch2
