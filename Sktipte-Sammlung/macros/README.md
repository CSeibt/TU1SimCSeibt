All parameters are given with their standard values:

#geometry/Shielding/enable True/False
#geometry/TU1Detector/enable True/False
enables or disables the shielding geometry



every following setting has prefix /geometry/TU1Detector/...



#detectorLoc 5 cm
alters the height at which the detector setup is located within the shielding (by shifting the shielding)

#deadLayerFront 0.2 mm
#deadLayerOutside 0.2 mm
#deadLayerInside 0.2 mm
#deadLayerBack 0.2 mm
alters the thickness of the deadlayer at the surface facing the detector cap, the outer surface, the inner surface around the borehole and the surface facing away from the cap respectively

#distanceFromCap 7.5 mm
alters the distance between the detector endcap and the Ge crystal

#CapThickness 1.5 mm
alters the thickness of the Al cap

#crystalLength 90 mm
#CrystalDiameter 90 mm
alters the length and diameter of the crystal geometry

#boreHoleDiameter 15 mm
#boreHoleLength 60 mm
alters the diameter and the length of the bor hole in the crystal respectively

#sampleHolderOn True/False
enables or disables the sample holder geometry, consisting of the a copper plate holding a source and copper legs,  holding the top part

#holderRoundedCorners True/False
enables or disables rounded corners of the top part. if disabled, top part is sqare with pointy corners, if enabled, those corners are rounded and thus more similar to the real counterpart

#sampleDistance 7 cm
distance between the detector endcap and the middle of the center of the top part
(the copper legs are automatically adapted to this distance. an asumption is made, that the sorce is exactly in the middle of the top part)

#holderHoleOuterDiameter 3.35 cm
#holderHoleInnerDiameter 2.02 cm
alters the outer and inner diameter of the cutout in the holder top part. Use values corresponding to used rings in real measurement


#coolingFingerOn True/False
enables or disables copper cool finger filling the borehole

#coolingFingerLength 14.5 cm
the length of the cooling finger, starting from the inner borehole bound


#innerCopperRing True/False
enables or disables cylindrical copper shielding around germanium detector

#crystalHolderOn True/False
enables or disables aluminium crystal holder surrounding the crystal


#extendedSampleOn True/False
enables or disables extended source geometry. Specific Shape can be changed with extencedSampleShape

#extendedSampleShape cylinder
Select Shape of extended Sample Geometry. Implemented are "cylinder" and "sphere"
cylinder: Geometry consists of hollow cylindrical plastic casing, in which a material is contained.
sphere: a basic loose sphere which's diameter and center can be changed

CYLINDER SPECIFIC OPTIONS:
	#plasticInnerDiameter 6.93 cm
	alters the diameter of the hollow inside 
	
	#plasticInnerHeight 2.175 cm
	alters the height of the hollow inside 

	#plasticOuterDiameter 7.125 cm
	alters the diameter of the outside 
	
	#plasticOuterHeight 2.36 cm
	alters the height of the outside
	
	#sampleOffset 0 cm
	The extended sample is automatically placed on the cu sample holder. If e.g. the extended source is too big for holder and want to place it on top, set offset of 2.5 mm to shift upwards.

SPHERE SPECIFIC OPTIONS:
	#sphereDiameter 2.5 cm
	alters the diameter of the sphere
	
	#sphereCenter 1.25 cm
	change the z-position of the sphere along the middle axis of the detector

#extendedSampleFilling 5 mm
set the filling of the material in the plastic casing

#sampleMaterialName lorandit
set material used in fillig. Own defined materials: lorandit, K2SO4

#sampleMaterialDensity 1 g/cm3
alter density of material


