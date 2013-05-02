# TODO List #


## jscam ##

### High Level ###
 * Refine js api (some functions can be consolidated)
 * Test harnesses and drivers to validate cxxcam functions
 * Bind Tool loading from json (pending work in cxxcam)
 * JS function library
 * Bind functions to retrieve generated GCode

### Code ###
 * Catch cxxcam exceptions and propagate as js exceptions

## cxxcam ##

### High Level ###
 * Auto feedrate / spindle speeds
    - Enhance spindle to report spindle speed requests outside of some specified tolerance. (i.e. 30,000rpm request for machine with max 500rpm)
 * Complete Tool class
    - Needs nef model loading / generation
    - Store properties for different tool types
 * Complete Stock class
    - Needs model loading & generation
    - Needs intersection tests
    - Needs material properties
 * Represent workholding, clamps, etc. (for intersection tests)
 * Toolpath expansion & intersection tests for rapids
 * Material removal volume for each operation
    - Calculate volume of material that tool can remove and ensure that volume is appropriate.
 * Arc motion
 * Plunge motion (Primitive function?)
 * Restore position
    - Ensure move is safe (no intersection with tool or clamps)
 * GCode Generation
    - Export generated GCode as lines and sequences of words (needed for js post-processing)
    - Export generated GCode as string
    - Aligned comments within a gcode block
    - Generate according to Variant (e.g. gcode case and precision are unimplemented)

### Code ###
 * Ensure exceptions do not change state (to allow recovery)
 * Change private / protected static methods to free functions.
 * Formalise result of nef glide when result is not 2-manifold

