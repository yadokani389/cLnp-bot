{
  inputs = { nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable"; };
  outputs = { nixpkgs, ... }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      cLnp-bot = with pkgs;
        stdenv.mkDerivation {
          pname = "cLnp-bot";

          version = "1.0.0";

          src = ./.;

          installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            cp cLnp-bot $out/bin

            runHook postInstall
          '';

          nativeBuildInputs = [ cmake ];

          buildInputs = [ dpp ];
        };
    in { defaultPackage."x86_64-linux" = cLnp-bot; };
}
