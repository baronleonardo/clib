{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  hardeningDisable = [ "all" ];
  buildInputs = with pkgs; [
    cmake
    gdb
    ninja
    valgrind
    zig
    wine64Packages.minimal
  ];

  shellHook = ''
  '';
}