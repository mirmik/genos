from ralgo.libralgo import * 
import ralgo.libralgo
import math
import scipy
import numpy
from scipy.spatial.transform import Rotation

class pose2(ralgo.libralgo.pose2):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)

	def numpy_rotmat(self):
		ang = self.ang
		c = math.cos(ang)
		s = math.sin(ang)
		r = numpy.array([
			[c, -s],
			[s,  c]
		])
		return r

	def numpy_vecbivec_rotmat(self):
		A00 = self.numpy_rotmat()
		return scipy.linalg.block_diag(A00, 1)
