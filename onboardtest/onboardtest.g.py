from xml.etree.ElementInclude import include
import licant

licant.module("onboardtest",
              include_paths=["."],
              sources=["onboardtest/testenv.cpp"],
              )
