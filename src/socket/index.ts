import type { Server, Socket } from "socket.io";
import { logger } from "../utils/logger.utils";

type Room = {
  owner: string,
  name: string,
  maxPlayers: number,
  players: Array<string>
}

const rooms: Array<Room> = [];

export const loadIoListeners = (io: Server) => {
  const disconnectFromRoom = (socket: Socket) => {
    const roomName = socket.data.room
    const username = socket.data.username
    const i = rooms.findIndex(r => r.name == roomName)

    if (i != -1) {
      const room = rooms[i]
      room.players = room.players.filter(player => player != username)

      socket.data.room = null
      socket.leave(roomName)

      if (room.players.length == 0) {
        rooms.splice(i, 1)
        logger.info(`The room ${room.name} removed`)
      }

      else {
        if (room.owner == username) {
          room.owner = room.players[0]
        }
        io.to(room.name).emit("updateRoom", JSON.stringify(room))
      }

      logger.info(`${username} disconnected from ${roomName}`)
      socket.emit("receiveDisconnectFromRoom")
    }
  }

  io.on("connection", (socket: Socket) => {
    socket.on("authenticate", (username: string) => {
      socket.data.username = username
      logger.info(`${username} connected`)
    })

    socket.on("getRooms", () => {
      socket.emit("receiveRooms", JSON.stringify({ rooms }))
    })

    socket.on("createRoom", (json: string) => {
      const data: Omit<Room, "players" | "owner"> = JSON.parse(json);
      const username = socket.data.username;

      const room: Room = {
        ...data,
        owner: username,
        players: [username]
      }

      rooms.push(room)
      socket.join(data.name)
      socket.data.room = data.name

      logger.info(`The room ${room.name} created`)
      socket.emit("updateRoom", JSON.stringify(room))
    })

    socket.on("disconnectFromRoom", () => {
      disconnectFromRoom(socket)
    })

    socket.on("connectToRoom", (roomName: string) => {
      const room = rooms.find(r => r.name == roomName)
      const username = socket.data.username

      if (!room) {
        return socket.emit("error", "The room not found")
      }

      else if (room.players.length == room.maxPlayers) {
        return socket.emit("error", "The room is full")
      }
      room.players.push(username)

      logger.info(`${username} connected to ${roomName}`)
      socket.data.room = roomName
      socket.join(roomName)
      io.to(roomName).emit("updateRoom", JSON.stringify(room))
    })

    socket.on("disconnect", () => {
      disconnectFromRoom(socket)
      const username = socket.data.username
      logger.info(`${username} disconnected`)
    })
  })
}
