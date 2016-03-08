# Development tools
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" #install brew
brew install libtool
brew install automake
brew install cmake
brew install wget

# Install googletest framework
wget http://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip 
rm -rf gtest-1.7.0.zip
cd gtest-1.7.0
./configure
sudo cp -a include/gtest /usr/local/include    
sudo cp -a lib/.libs/* /usr/local/lib/       
cd ..
rm -rf gtest-1.7.0

# Ragel and dot
brew install ragel
brew install graphviz