/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   blueRecord_png;
    const int            blueRecord_pngSize = 163493;

    extern const char*   redRecord_png;
    const int            redRecord_pngSize = 306944;

    extern const char*   stopbutton_png;
    const int            stopbutton_pngSize = 5083;

    extern const char*   pausebutton_png;
    const int            pausebutton_pngSize = 5012;

    extern const char*   speedSliderDial_png;
    const int            speedSliderDial_pngSize = 7176;

    extern const char*   playbutton_png;
    const int            playbutton_pngSize = 4747;

    extern const char*   volumeMixerDial_png;
    const int            volumeMixerDial_pngSize = 7267;

    extern const char*   sliderDial_png;
    const int            sliderDial_pngSize = 6044;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
