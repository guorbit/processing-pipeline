# Start with a base image that includes DeepStream and CUDA tailored for ARM64
FROM nvcr.io/nvidia/deepstream-l4t:6.0-samples as deepstream_img

# Use CUDA base image
FROM nvcr.io/nvidia/l4t-cuda:10.2.460-runtime

# Copy DeepStream from the first image
COPY --from=deepstream_img /opt/nvidia/deepstream/deepstream-6.0/ /opt/nvidia/deepstream/deepstream-6.0/

# Install necessary libraries for compilation
RUN apt-get update && apt-get install -y \
    libgstreamer-plugins-base1.0-dev \
    libgstreamer1.0-dev \
    libgstrtspserver-1.0-dev \
    libx11-dev

# Remove the existing g++ package to avoid conflicts
RUN apt-get purge -y g++

RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install -y g++-9
# Set g++-9 as the default g++ version
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9  60
# Create symbolic links for CUDA libraries (adjust paths as necessary for your CUDA version)
RUN ln -s /usr/local/cuda-10.2/targets/aarch64-linux/lib/stubs/libcuda.so /usr/local/cuda-10.2/lib64/libcuda.so && \
    ln -s /usr/local/cuda-10.2/lib64/libcudart.so.10.2 /usr/local/cuda-10.2/lib64/libcudart.so



WORKDIR /opt/nvidia/deepstream/deepstream-6.0
CMD ["/bin/bash"]
