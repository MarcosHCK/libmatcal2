#! /bin/sh

mkdir_s ()
{
  local dir=$1
  if ! test -d $dir; then
    mkdir $dir
  fi
}

copy_classlib ()
{
  local framework target

  framework=$1
  target=$2

  mkdir_s lib/$framework/
  cp -f ../MatcalSharp/bin/$target/$framework/MatcalSharp.dll lib/$framework/
  if test "x$target" = "xDebug"; then
    cp -f ../MatcalSharp/bin/$target/$framework/MatcalSharp.pdb lib/$framework/
  fi
}

copy_runtime ()
{
  local name srcdir system arch

  name=$1
  srcdir=$2
  system=$3
  arch=$4

  mkdir_s runtimes/$system-$arch/
  mkdir_s runtimes/$system-$arch/native/
  cp -f $srcdir/.libs/lib$name*.so* runtimes/$system-$arch/native/
}

mkdir_s "lib"
mkdir_s "runtimes"

copy_classlib "net6.0" "Debug"
copy_runtime "matcal" "../../" "linux" "x64"
copy_runtime "matlib" "../../" "linux" "x64"
copy_runtime "matcal_glue" "../" "linux" "x64"
