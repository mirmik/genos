import ralgo

class rotation_link(ralgo.libralgo.rotation_link):
	def __init__(self, v):
		super().__init__(ralgo.to_vec3(v))

class translation_link(ralgo.libralgo.translation_link):
	def __init__(self, v):
		super().__init__(ralgo.to_vec3(v))

class chain(ralgo.libralgo.chain):
	def __init__(self, lst):
		super().__init__(lst)
