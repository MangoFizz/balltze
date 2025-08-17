FROM archlinux:base as base

LABEL org.opencontainers.image.description="Development environment for Balltze"

# Create devel user...
ARG USER_ID=1001
ARG GROUP_ID=1001
RUN groupadd -g ${GROUP_ID} devel && \
    useradd -m -d /home/devel -u ${USER_ID} -g ${GROUP_ID} -G users,tty -s /bin/bash devel
RUN echo 'devel ALL=(ALL) NOPASSWD: /usr/sbin/pacman, /usr/sbin/makepkg' >> /etc/sudoers;

RUN mkdir -p /workdir && chown devel:users /workdir

#Workaround for the "setrlimit(RLIMIT_CORE): Operation not permitted" error
RUN echo "Set disable_coredump false" >> /etc/sudo.conf

# Set up keyring
RUN pacman-key --init
RUN pacman-key --populate archlinux
RUN pacman -Sy --noconfirm --disable-sandbox archlinux-keyring

# Update the system
RUN pacman -Syu --noconfirm --disable-sandbox 

# Add packages to the base system
RUN pacman -S --noconfirm --disable-sandbox \
        base-devel cmake git go wine patch wget \
        mingw-w64 ninja expac nano openssh vim lua53

ENV EDITOR=nano

# Install yay
USER devel
ARG BUILDDIR=/tmp/tmp-build
RUN mkdir "${BUILDDIR}" && cd "${BUILDDIR}" && \
    git clone https://aur.archlinux.org/yay.git && \
    cd yay && makepkg -si --noconfirm --rmdeps && \
    rm -rf "${BUILDDIR}"

ENV HOME=/home/devel

# Install AUR packages
USER devel
RUN yay -S --noconfirm --needed \
        mingw-w64-libvorbis \
        mingw-w64-flac \
        mingw-w64-libbacktrace-debuglink-git

# Cleanup
USER root
RUN pacman -Scc --noconfirm --disable-sandbox
RUN rm -rf /usr/share/man/*; \
    rm -rf /tmp/*; \
    rm -rf /var/tmp/*;
USER devel
RUN yay -Scc
