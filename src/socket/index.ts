import type { Server } from "socket.io";
import { logger } from "../utils/logger.utils";

type User = {
  socket: string,
  username: string
}

type Room = {
  owner: User,
  name: string,
  maxPlayers: number,
  players: Array<User>
}

const rooms: Array<Room> = [];

export const loadIoListeners = (io: Server) => {
  io.on("connection", (socket) => {
    logger.info(`User ${socket.id} was connected`)

    socket.on("getRooms", () => {
      const names = rooms.map(room => room.name)
      socket.emit("receiveRooms", JSON.stringify({ rooms: names }))
    })

    socket.on("createRoom", (s: string) => {
      const data: Omit<Room, "players"> = JSON.parse(s);
      const room: Room = {
        ...data,
        players: [data.owner]
      }
      rooms.push(room)
      socket.emit("receiveCreateRoom", JSON.stringify(room))
    })
  })
}
