# XInputFloodingTest
Tests how often one can send inputs to XInputSetState (which is buggy when using an XBox One Controller)

Force Feedback seems to be sending fine, until we go lower than 8ms. Then the commands seem to get messed up and force feedback is no longer applied correctly.
