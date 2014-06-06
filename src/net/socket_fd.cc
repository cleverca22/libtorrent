// libTorrent - BitTorrent library
// Copyright (C) 2005-2007, Jari Sundell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// In addition, as a special exception, the copyright holders give
// permission to link the code of portions of this program with the
// OpenSSL library under certain conditions as described in each
// individual source file, and distribute linked combinations
// including the two.
//
// You must obey the GNU General Public License in all respects for
// all of the code used other than OpenSSL.  If you modify file(s)
// with this exception, you may extend this exception to your version
// of the file(s), but you are not obligated to do so.  If you do not
// wish to do so, delete this exception statement from your version.
// If you delete this exception statement from all source files in the
// program, then also delete it here.
//
// Contact:  Jari Sundell <jaris@ifi.uio.no>
//
//           Skomakerveien 33
//           3185 Skoppum, NORWAY

#include "config.h"

#include <errno.h>
#include <fcntl.h>
#ifdef WIN32
# include <winsock2.h>
#else
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/in_systm.h>
# include <netinet/ip.h>
#endif
#include <rak/socket_address.h>
#include <rak/error_number.h>

#include "torrent/exceptions.h"
#include "socket_fd.h"

namespace torrent {

inline void
SocketFd::check_valid() const {
  if (!is_valid())
    throw internal_error("SocketFd function called on an invalid fd.");
}

bool
SocketFd::set_nonblock() {
  check_valid();
#ifdef WIN32
  u_long one = 1;
  return ioctlsocket(m_fd,FIONBIO,&one) == 0;
#else
  return fcntl(m_fd, F_SETFL, O_NONBLOCK) == 0;
#endif
}

bool
SocketFd::set_priority(priority_type p) {
  check_valid();
  int opt = p;

  return setsockopt(m_fd, IPPROTO_IP, IP_TOS, (char*)&opt, sizeof(opt)) == 0;
}

bool
SocketFd::set_reuse_address(bool state) {
  check_valid();
  int opt = state;

  return setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == 0;
}

bool
SocketFd::set_send_buffer_size(uint32_t s) {
  check_valid();
  int opt = s;

  return setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, (char*)&opt, sizeof(opt)) == 0;
}

bool
SocketFd::set_receive_buffer_size(uint32_t s) {
  check_valid();
  int opt = s;
  return setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (char*)&opt, sizeof(opt)) == 0;
}

int
SocketFd::get_error() const {
  check_valid();

  int err;
  socklen_t length = sizeof(err);

  if (getsockopt(m_fd, SOL_SOCKET, SO_ERROR, (char*)&err, &length) == -1)
    throw internal_error("SocketFd::get_error() could not get error");

  return err;
}

bool
SocketFd::open_stream() {
#ifdef WIN32
  return (m_fd = socket(rak::socket_address::pf_inet, SOCK_STREAM, IPPROTO_TCP)) != INVALID_SOCKET;
#else
  return (m_fd = socket(rak::socket_address::pf_inet, SOCK_STREAM, IPPROTO_TCP)) != -1;
#endif
}

bool
SocketFd::open_datagram() {
  return (m_fd = socket(rak::socket_address::pf_inet, SOCK_DGRAM, 0)) != -1;
}

bool
SocketFd::open_local() {
  return (m_fd = socket(rak::socket_address::pf_local, SOCK_STREAM, 0)) != -1;
}

void
SocketFd::close() {
  int ret;
#ifdef WIN32
  //printf("SocketFd::close() m_socket == %d\n",m_fd);
  ret = ::closesocket(m_fd);
    int winerr = WSAGetLastError();
    if (winerr == WSAEWOULDBLOCK) return;
    if (winerr == ERROR_INVALID_PARAMETER) return;
    //printf("winerr %d, errno %d, ret=%d\n",winerr,errno,ret);
    rak::error_number err(winerr);
    // FIXME, throw errors?
#else
  ret = ::close(m_fd);
  rak::error_number err = rak::error_number::current();
  if (ret && errno == EBADF)
    throw internal_error("SocketFd::close() called on an invalid file descriptor");
#endif
}

bool
SocketFd::bind(const rak::socket_address& sa) {
  check_valid();

  return !::bind(m_fd, sa.c_sockaddr(), sa.length());
}

bool
SocketFd::bind(const rak::socket_address& sa, unsigned int length) {
  check_valid();

  return !::bind(m_fd, sa.c_sockaddr(), length);
}

bool
SocketFd::connect(const rak::socket_address& sa) {
  check_valid();

#ifdef WIN32
  //printf("sock fd connect %d %s\n",m_fd,sa.address_str().c_str());
  bool ret = !::connect(m_fd, sa.c_sockaddr(), sa.length()); // FIXME?
  int winerr = WSAGetLastError();
  if (winerr == WSAEWOULDBLOCK) return true;
  //printf("winerr=%d errno = %d %s\n",winerr,errno,strerror(errno));
  return ret;
#else
  return !::connect(m_fd, sa.c_sockaddr(), sa.length()) || errno == EINPROGRESS;
#endif
}

bool
SocketFd::listen(int size) {
  check_valid();

  return !::listen(m_fd, size);
}

SocketFd
SocketFd::accept(rak::socket_address* sa) {
  check_valid();
  socklen_t len = sizeof(rak::socket_address);

  return SocketFd(::accept(m_fd, sa != NULL ? sa->c_sockaddr() : NULL, &len));
}

// unsigned int
// SocketFd::get_read_queue_size() const {
//   unsigned int v;

//   if (!is_valid() || ioctl(m_fd, SIOCINQ, &v) < 0)
//     throw internal_error("SocketFd::get_read_queue_size() could not be performed");

//   return v;
// }

// unsigned int
// SocketFd::get_write_queue_size() const {
//   unsigned int v;

//   if (!is_valid() || ioctl(m_fd, SIOCOUTQ, &v) < 0)
//     throw internal_error("SocketFd::get_write_queue_size() could not be performed");

//   return v;
// }

}
