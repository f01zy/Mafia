import type { Server, Socket } from "socket.io";
import { logger } from "../utils/logger.utils";

type Role = "mafia" | "don" | "doctor" | "sheriff" | "civilian"

type Message = {
  username: string,
  content: string
}

type Player = {
  username: string,
  role: Role
}

type Room = {
  owner: string,
  name: string,
  maxPlayers: number,
  players: Array<Player>,
  messages: Array<Message>,
  state: "day" | "night" | "not started"
}

const rooms: Array<Room> = []

export const loadIoListeners = (io: Server) => {
  const disconnectFromRoom = (socket: Socket) => {
    const currentRoom = socket.data.currentRoom
    const username = socket.data.username
    const roomIndex = rooms.findIndex(room => room.name == currentRoom)

    if (roomIndex === -1) {
      return socket.emit("error", "Room not found");
    }

    const room = rooms[roomIndex]
    room.players = room.players.filter(player => player.username != username)

    socket.data.currentRoom = null
    socket.leave(currentRoom)

    if (room.players.length == 0) {
      rooms.splice(roomIndex, 1)
      logger.info(`The room \"${currentRoom}\" removed`)
    }

    else {
      room.messages.push({
        username: "System",
        content: `The user \"${username}\" disconnected`
      })

      if (room.owner == username) {
        room.owner = room.players[0].username
      }

      io.to(currentRoom).emit("updateRoom", JSON.stringify(room))
    }

    socket.emit("receiveDisconnectFromRoom")
    logger.info(`The user \"${username}\" disconnected from \"${currentRoom}\"`)
  }

  io.on("connection", (socket: Socket) => {
    socket.on("authenticate", (username: string) => {
      socket.data.username = username
      logger.info(`The user \"${username}\" connected`)
    })

    socket.on("getRooms", () => {
      const notStartedRooms = rooms.filter(room => room.state == "not started")
      socket.emit("receiveRooms", JSON.stringify({ rooms: notStartedRooms }))
    })

    socket.on("createRoom", (json: string) => {
      const data: Omit<Room, "users" | "owner" | "state"> = JSON.parse(json);
      const username = socket.data.username;

      const room: Room = {
        ...data,
        owner: username,
        players: [{
          username,
          role: "civilian"
        }],
        messages: [],
        state: "not started"
      }

      rooms.push(room)
      socket.join(data.name)
      socket.data.currentRoom = data.name
      socket.emit("updateRoom", JSON.stringify(room))
      logger.info(`The room \"${room.name}\" created`)
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

      room.players.push({
        username,
        role: "civilian"
      })

      socket.data.currentRoom = name
      socket.join(name)
      io.to(name).emit("updateRoom", JSON.stringify(room))
      logger.info(`The user \"${username}\" connected to \"${name}\"`)
    })

    socket.on("startGame", () => {
      const currentRoom = socket.data.currentRoom;
      const roomIndex = rooms.findIndex(room => room.name === currentRoom);

      if (roomIndex == -1) {
        return socket.emit("error", "Room not found");
      }

      const room = rooms[roomIndex];

      if (room.state != "not started") {
        return socket.emit("error", "Game has already started");
      }

      if (room.owner !== socket.data.username) {
        return socket.emit("error", "Only the room owner can start the game");
      }

      const playerCount = room.players.length;

      const roleDistribution: Array<{ role: Role; count: number }> = [
        { role: "doctor", count: 1 },
        { role: "sheriff", count: 1 },
        { role: "don", count: 1 },
        { role: "mafia", count: Math.max(1, Math.floor(playerCount / 4)) },
      ];

      const totalRoles = roleDistribution.reduce((sum, r) => sum + r.count, 0);

      if (totalRoles > playerCount) {
        return socket.emit("error", "Not enough players to assign all roles");
      }

      const shuffledPlayers = [...room.players];
      const rolesToAssign: Array<{ role: Role }> = [];

      for (const { role, count } of roleDistribution) {
        for (let i = 0; i < count; i++) {
          rolesToAssign.push({ role });
        }
      }

      for (let i = shuffledPlayers.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [shuffledPlayers[i], shuffledPlayers[j]] = [shuffledPlayers[j], shuffledPlayers[i]];
      }

      for (let i = 0; i < rolesToAssign.length; i++) {
        shuffledPlayers[i].role = rolesToAssign[i].role;
      }

      for (let i = rolesToAssign.length; i < shuffledPlayers.length; i++) {
        shuffledPlayers[i].role = "civilian";
      }

      room.players = shuffledPlayers;
      room.state = "day";

      io.to(currentRoom).emit("updateRoom", JSON.stringify(room));
      logger.info(`Game "${currentRoom}" started with ${playerCount} players`);
    })

    socket.on("sendMessage", (content: string) => {
      const currentRoom = socket.data.currentRoom
      const username = socket.data.username
      const i = rooms.findIndex(room => room.name == currentRoom)

      if (i != -1) {
        const room = rooms[i]
        room.messages.push({
          username: username,
          content
        })

        io.to(currentRoom).emit("updateRoom", JSON.stringify(room));
        logger.info(`Message: \"${content}\"`);
      }
    })

    socket.on("disconnect", () => {
      disconnectFromRoom(socket)
      const username = socket.data.username
      logger.info(`The user \"${username}\" disconnected`)
    })
  })
}
