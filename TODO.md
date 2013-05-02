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
 * Export generated GCode as lines and sequences of words (needed for js post-processing)
    - Export generated GCode as string

### Code ###
 * Ensure exceptions do not change state (to allow recovery)

