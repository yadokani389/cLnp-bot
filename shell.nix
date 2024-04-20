{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    cmake
    gcc
    dpp
    openssl
    zlib
    libopus
    libsodium
  ];
}
