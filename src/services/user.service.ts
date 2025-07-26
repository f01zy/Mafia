import { ApiError } from "../exceptions/api.exception"
import { prisma } from "../prisma"
import { MailService } from "./mail.service"
import { TokenService } from "./token.service"
import bcrypt from "bcrypt"
import crypto from "crypto"

const mailService = new MailService()
const tokenService = new TokenService()

export class UserService {
  public async register(username: string, email: string, password: string) {
    const candidateEmail = await prisma.user.findFirst({ where: { email } })
    if (candidateEmail) {
      throw ApiError.BadRequest("The user with this email already exists")
    }

    const candidateUsername = await prisma.user.findFirst({ where: { username } })
    if (candidateUsername) {
      throw ApiError.BadRequest("The user with this username already exists")
    }

    const hashPassword = await bcrypt.hash(password, 3)
    const activationCode = crypto.randomUUID();

    // await mailService.sendActivationMail(email, activationCode)
    const user = await prisma.user.create({ data: { username, email, password: hashPassword, activationCode } })

    const tokens = await tokenService.generateTokens(user.id)
    await tokenService.saveToken(user.id, tokens.refreshToken)

    return {
      ...tokens,
      user: user
    }
  }

  public async login(username: string, password: string) {
    const user = await prisma.user.findFirst({ where: { username } })
    if (!user) {
      throw ApiError.BadRequest("The user with this username was not found")
    }

    const isPasswordCorrect = await bcrypt.compare(password, user.password)
    if (!isPasswordCorrect) {
      throw ApiError.BadRequest("Incorrect password")
    }

    const tokens = await tokenService.generateTokens(user.id)
    await tokenService.saveToken(user.id, tokens.refreshToken)

    return {
      ...tokens,
      user: user
    }
  }

  public async logout(refreshToken: string) {
    const token = await tokenService.removeToken(refreshToken)
    return token
  }

  public async activate(activationCode: string) {
    const user = await prisma.user.findFirst({ where: { activationCode } })
    if (!user) {
      throw ApiError.BadRequest("Non-direct activation code")
    }

    user.isActivated = true
  }

  public async refresh(refreshToken: string) {
    const user = await tokenService.getUserByRefreshToken(refreshToken)

    await tokenService.removeToken(refreshToken);
    const tokens = await tokenService.generateTokens(user.id)
    await tokenService.saveToken(user.id, tokens.refreshToken)

    return {
      ...tokens,
      user: user
    }
  }
}
