import type { Server, Socket } from "socket.io";
import { logger } from "../utils/logger.utils";

type Message = {
  username: string,
  message: string
}

type Room = {
  owner: string,
  name: string,
  maxPlayers: number,
  players: Array<string>,
  messages: Array<Message>,
  state: "day" | "night" | "not started"
}

const rooms: Array<Room> = []

export const loadIoListeners = (io: Server) => {
  const disconnectFromRoom = (socket: Socket) => {
    const currentRoom = socket.data.currentRoom
    const username = socket.data.username
    const i = rooms.findIndex(room => room.name == currentRoom)

    if (i != -1) {
      const room = rooms[i]
      room.players = room.players.filter(player => player != username)

      socket.data.currentRoom = null
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
      socket.emit("receiveDisconnectFromRoom")
      logger.info(`The user ${username} disconnected from ${currentRoom}`)
    }
  }

  io.on("connection", (socket: Socket) => {
    socket.on("authenticate", (username: string) => {
      socket.data.username = username
      logger.info(`The user ${username} connected`)
    })

    socket.on("getRooms", () => {
      const notStartedRooms = rooms.filter(room => room.state == "not started")
      socket.emit("receiveRooms", JSON.stringify({ rooms: notStartedRooms }))
    })

    socket.on("createRoom", (json: string) => {
      const data: Omit<Room, "players" | "owner" | "time"> = JSON.parse(json);
      const username = socket.data.username;

      const room: Room = {
        ...data,
        owner: username,
        players: [username],
        messages: [],
        state: "not started"
      }

      rooms.push(room)
      socket.join(data.name)
      socket.data.currentRoom = data.name
      socket.emit("updateRoom", JSON.stringify(room))

      logger.info(`The room ${room.name} created`)
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

      socket.data.currentRoom = name
      socket.join(name)
      io.to(name).emit("updateRoom", JSON.stringify(room))

      logger.info(`The user ${username} connected to ${name}`)
    })

    socket.on("startGame", () => {
      const currentRoom = socket.data.currentRoom
      const i = rooms.findIndex(room => room.name == currentRoom)

      if (i != -1) {
        const room = rooms[i]
        room.state = "day"

        io.to(currentRoom).emit("receiveStartGame")
        logger.info(`Game ${currentRoom} started`)
      }
    })

    socket.on("sendMessage", (message: string) => {
      const currentRoom = socket.data.currentRoom
      const username = socket.data.username
      const i = rooms.findIndex(room => room.name == currentRoom)

      if (i != -1) {
        const room = rooms[i]
        room.messages.push({
          username: username,
          message
        })

        io.to(currentRoom).emit("updateRoom", JSON.stringify(room));
        logger.info(`Message: ${message}`);
      }
    })

    socket.on("disconnect", () => {
      disconnectFromRoom(socket)
      const username = socket.data.username
      logger.info(`The user ${username} disconnected`)
    })
  })
}
