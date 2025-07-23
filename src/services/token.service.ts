import jwt from "jsonwebtoken"
import { Variables } from "../env/variables.env"
import { prisma } from "../prisma"
import { ApiError } from "../exceptions/api.exception"

export class TokenService {
  public async validateRefresh(token: string) {
    try {
      const userToken = jwt.verify(token, Variables.JWT_REFRESH_SECRET) as { id: number }
      const user = await prisma.user.findUnique({ where: { id: userToken.id } })

      return user
    } catch (e) {
      return null
    }
  }

  public async generateTokens(id: number) {
    const accessToken = jwt.sign({ id }, Variables.JWT_ACCESS_SECRET, { expiresIn: "30m" })
    const refreshToken = jwt.sign({ id }, Variables.JWT_REFRESH_SECRET, { expiresIn: "30d" })

    return {
      accessToken,
      refreshToken
    }
  }

  public async saveToken(userId: number, refreshToken: string) {
    const token = await prisma.token.findFirst({ where: { userId } })
    if (token) {
      token.refreshToken = refreshToken
    }
    const tokenCreated = await prisma.token.create({ data: { userId, refreshToken } })

    return tokenCreated
  }

  public async removeToken(refreshToken: string) {
    const user = await this.getUserByRefreshToken(refreshToken)
    const token = await prisma.token.deleteMany({ where: { userId: user.id } })

    return token
  }

  public async findToken(refreshToken: string) {
    const token = await prisma.token.findFirst({ where: { refreshToken } })

    return token
  }

  public async getUserByRefreshToken(refreshToken: string) {
    if (!refreshToken) {
      throw ApiError.UnauthorizedError()
    }

    const userData = await this.validateRefresh(refreshToken)
    const tokenDb = await this.findToken(refreshToken)
    if (!tokenDb || !userData) {
      throw ApiError.UnauthorizedError()
    }

    const user = await prisma.user.findUnique({ where: { id: userData.id } })
    if (!user) {
      throw ApiError.BadRequest("Invalid token.")
    }

    return user
  }
}
