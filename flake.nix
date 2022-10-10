{
  description = "cstdb devshell";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in
  {
    # packages.x86_64-linux.hello = nixpkgs.legacyPackages.x86_64-linux.hello;
    devShells.${system}.default = import ./shell.nix { inherit pkgs; };
  };
}
