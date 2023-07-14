FROM archlinux:base as base

# Create devel user...
RUN useradd -m -d /home/devel -u 1000 -U -G users,tty -s /bin/bash devel
RUN echo 'devel ALL=(ALL) NOPASSWD: /usr/sbin/pacman, /usr/sbin/makepkg' >> /etc/sudoers;

RUN mkdir -p /workdir && chown devel.users /workdir

#Workaround for the "setrlimit(RLIMIT_CORE): Operation not permitted" error
RUN echo "Set disable_coredump false" >> /etc/sudo.conf

RUN pacman -Sy --noconfirm --noprogressbar archlinux-keyring
RUN pacman-key --init
RUN pacman-key --populate archlinux

# Update the system
RUN pacman -Syu --noconfirm --noprogressbar

# Add packages to the base system
RUN pacman -S --noconfirm --noprogressbar \
        base-devel cmake git mingw-w64 \
        patch wget \
        expac nano openssh vim

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
RUN yay -S --noconfirm --noprogressbar --needed \
        mingw-w64-libvorbis \
        mingw-w64-flac \
        mingw-w64-cmake \
        lua53

# Cleanup
USER root
RUN pacman -Scc --noconfirm
RUN paccache -r -k0; \
    rm -rf /usr/share/man/*; \
    rm -rf /tmp/*; \
    rm -rf /var/tmp/*;
USER devel
RUN yay -Scc
