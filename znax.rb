class Znax < Formula
  desc "Znax is a cross platform puzzle / arcade game using SDL2 libraries"
  homepage "https://joyrider3774.github.io/Znax/"
  url "https://github.com/joyrider3774/Znax/archive/refs/tags/v1.1.tar.gz"
  sha256 "939da6461e7fded69640cb868f80e7da4cb969c43e6727c07f80f8d3e4b9c7fe"
  license "MIT"

  depends_on "make" => :build
  depends_on "sdl2"
  depends_on "sdl2_ttf"
  depends_on "sdl2_mixer"
  depends_on "sdl2_image"
  depends_on "sdl2_gfx"
  
  def install
    system "make","CFLAGS=\"-I#{HOMEBREW_PREFIX}/include/SDL2\"", "LDFLAGS=\"-L#{HOMEBREW_PREFIX}/lib\""
    prefix.install "znax"
    prefix.install "znaxfs"
    bin.write_exec_script (prefix/"znax")
  end

end
