# Four-aspect signal speed prototype

Based on JGRPP **0.73.4**, branch `four-aspect-v1`.

Download a successful Windows build from [Releases](../../releases), extract it into a separate folder, and run **PLAY-4ASPECT.cmd**.
See [Korean instructions and scope](prototype/README_4ASPECT_KO.md).

This is an experimental **aspect speed-control layer on existing JGR path reservations**, not a replacement fixed-block interlocking engine.
Native aspects 0/1/2/3+ mean stop / 45 / 65 / unrestricted; original route locking, reservation-end braking and rear release remain.
Default electric signals carry additional command lamps. Custom signal styles and simulated bridge/tunnel signals retain native handling.
The feature is off by default outside the provided portable config. New savegame feature metadata deliberately prevents unpatched games from silently loading the new train state.

The Windows workflow builds the complete game, runs CTest, packages its dependencies and OpenGFX, then creates a release.
A failed build never publishes a playable ZIP. Read CI status before downloading.
