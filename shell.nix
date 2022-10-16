{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    gdb
    ninja
    valgrind
    pkgsCross.mingwW64.buildPackages.gcc
  ];

  shellHook = ''
  '';
}