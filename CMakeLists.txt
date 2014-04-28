cmake_minimum_required(VERSION 2.8)

project(libtorrent)

add_library(libtorrent src/torrent/download.cc src/torrent/torrent.cc
src/torrent/throttle.cc src/download/download_wrapper.cc src/net/listen.cc
src/torrent/data/file.cc src/data/socket_file.cc src/data/hash_queue.cc
src/torrent/object_stream.cc src/torrent/object.cc src/torrent/http.cc
src/torrent/data/file_list.cc src/torrent/exceptions.cc src/torrent/path.cc
src/torrent/connection_manager.cc src/torrent/poll_select.cc src/torrent/rate.cc
src/torrent/download/resource_manager.cc src/manager.cc src/globals.cc
src/torrent/data/file_manager.cc src/download/download_main.cc src/data/chunk.cc
src/download/download_constructor.cc src/torrent/download/download_manager.cc
src/protocol/peer_factory.cc src/torrent/download.cc src/torrent/bitfield.cc
src/data/hash_torrent.cc src/tracker/tracker_manager.cc src/data/chunk_list.cc
src/torrent/data/download_data.cc src/torrent/download/choke_queue.cc
src/net/socket_set.cc src/torrent/thread_base.cc src/torrent/chunk_manager.cc
src/torrent/peer/client_list.cc src/protocol/handshake_manager.cc
src/net/socket_fd.cc src/net/socket_base.cc src/data/chunk_part.cc
src/torrent/peer/connection_list.cc src/torrent/peer/peer_list.cc
src/torrent/data/transfer_list.cc src/download/chunk_selector.cc
src/protocol/extensions.cc src/net/throttle_internal.cc src/dht/dht_node.cc
src/protocol/peer_connection_base.cc src/protocol/peer_connection_leech.cc
src/download/chunk_statistics.cc src/protocol/peer_connection_metadata.cc
src/data/hash_chunk.cc src/torrent/dht_manager.cc src/net/throttle_list.cc
src/protocol/handshake.cc src/torrent/peer/client_info.cc
src/net/socket_datagram.cc src/torrent/tracker.cc src/download/delegator.cc
src/dht/dht_transaction.cc src/dht/dht_tracker.cc src/utils/diffie_hellman.cc
src/tracker/tracker_http.cc src/tracker/tracker_dht.cc src/tracker/tracker_udp.cc
src/data/hash_queue_node.cc src/torrent/utils/log_files.cc
src/protocol/request_list.cc src/net/address_list.cc src/torrent/peer/peer.cc
src/protocol/initial_seed.cc src/protocol/peer_connection_base.cc
src/torrent/peer/peer_info.cc src/net/socket_stream.cc src/dht/dht_bucket.cc
src/download/available_list.cc src/data/memory_chunk.cc src/dht/dht_router.cc
src/torrent/data/block_list.cc src/torrent/data/block.cc
src/torrent/object_static_map.cc
src/torrent/download/choke_group.cc src/torrent/tracker_list.cc
src/protocol/handshake_encryption.cc src/dht/dht_server.cc)

include_directories(src/torrent src)
