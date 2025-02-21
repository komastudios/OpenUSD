#!/pxrpythonsubst
#
# Copyright 2017 Pixar
#
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.

# This test explicitly does *not* load usdGeom.  We're testing that core Usd
# functionality will work using only information loaded from usdGeom's
# plugInfo.json and generated schema, without loading the library.

from pxr import Plug, Usd
import unittest

class TestUsdGeomNoPlugLoad(unittest.TestCase):
    def test_Basic(self):
        # Find 'UsdGeomScope' by name, assert its size is unknown to TfType (this is
        # true since the library is not loaded and the type is only Declare()'d not
        # Define()'d.
        plugReg = Plug.Registry()
        scopeType = plugReg.FindDerivedTypeByName(Usd.Typed, 'UsdGeomScope')

        self.assertTrue(scopeType)
        self.assertEqual(scopeType.sizeof, 0, 
                         'Expected Declared() but not Defined() type')

        # Make a stage with a Scope, ensure we can type-check it.
        stage = Usd.Stage.CreateInMemory()
        scope = stage.DefinePrim('/scope', 'Scope')

        self.assertTrue(scope)
        self.assertTrue(scope.IsA(Usd.Typed))

        # Now ensure that fallbacks from generated schema apply without loading plugins.
        # Create a Cube prim and assert that size appears as an attribute with a
        # fallback value, even though there is no scene description for it.
        cube = stage.DefinePrim('/cube', 'Cube')
        self.assertTrue(cube)

        # Unauthored builtins should be reported by GetAttributes and GetProperties.
        self.assertTrue('size' in [attr.GetName() for attr in cube.GetAttributes()])
        self.assertTrue('size' in [prop.GetName() for prop in cube.GetProperties()])
        self.assertTrue('size' in cube.GetPropertyNames())

        # ... but should not appear for authored-only queries.
        self.assertTrue('size' not in [attr.GetName() for attr in cube.GetAuthoredAttributes()])
        self.assertTrue('size' not in [prop.GetName() for prop in cube.GetAuthoredProperties()])
        self.assertTrue('size' not in cube.GetAuthoredPropertyNames())

        # Fallback values should come from definition.
        sizeAttr = cube.GetAttribute('size')
        self.assertTrue(sizeAttr)
        self.assertEqual(sizeAttr.Get(), 2.0)
        self.assertFalse(sizeAttr.IsAuthored())

if __name__ == "__main__":
    unittest.main()
