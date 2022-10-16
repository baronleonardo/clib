{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    gdb
    ninja
    valgrind
    clang
  ];

  shellHook = ''
  '';
}