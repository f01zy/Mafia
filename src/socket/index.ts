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
    const currentRoom = socket.data.currentRoom
    const username = socket.data.username
    const i = rooms.findIndex(room => room.name == currentRoom)

    if (i != -1) {
      const room = rooms[i]
      room.players = room.players.filter(player => player != username)

      socket.data.room = null
      socket.leave(currentRoom)

      if (room.players.length == 0) {
        rooms.splice(i, 1)
        logger.info(`The room ${currentRoom} removed`)
      }

      else {
        if (room.owner == username) {
          room.owner = room.players[0]
        }
        io.to(currentRoom).emit("updateRoom", JSON.stringify(room))
      }

      logger.info(`${username} disconnected from ${currentRoom}`)
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
      socket.data.currentRoom = data.name

      logger.info(`The room ${room.name} created`)
      socket.emit("updateRoom", JSON.stringify(room))
    })

    socket.on("disconnectFromRoom", () => {
      disconnectFromRoom(socket)
    })

    socket.on("connectToRoom", (name: string) => {
      const room = rooms.find(r => r.name == name)
      const username = socket.data.username

      if (!room) {
        return socket.emit("error", "The room not found")
      }

      else if (room.players.length == room.maxPlayers) {
        return socket.emit("error", "The room is full")
      }
      room.players.push(username)

      logger.info(`${username} connected to ${name}`)
      socket.data.currentRoom = name
      socket.join(name)
      io.to(name).emit("updateRoom", JSON.stringify(room))
    })

    socket.on("startGame", () => {
      const currentRoom = socket.data.room

      const i = rooms.findIndex(r => r.name == currentRoom)
      if (i != -1) {
        rooms.splice(i, 1)
      }

      io.to(currentRoom).emit("receiveStartGame")
    })

    socket.on("disconnect", () => {
      disconnectFromRoom(socket)
      const username = socket.data.username
      logger.info(`${username} disconnected`)
    })
  })
}
